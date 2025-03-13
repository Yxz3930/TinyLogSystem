#include "AsyncLogging.h"
#include <string.h>
#include <iostream>

AsyncLogging::AsyncLogging(const char *logdir, int filesize, int interval, int write_num)
    : m_logfile(logdir, filesize, interval, write_num),
      MAX_BUFFER_SIZE(1024), m_running(true)
{
    this->active_bufferPtr = std::make_unique<Buffer_t>();
    this->standby_bufferPtr = std::make_unique<Buffer_t>();
    this->m_asyncThread = std::move(std::thread(&AsyncLogging::AsyncLoggingFunc, this));
}

AsyncLogging::~AsyncLogging()
{
    this->m_cv.notify_one();
    this->m_running = false;
    if (this->m_asyncThread.joinable())
        this->m_asyncThread.join();
}

void AsyncLogging::append(const char *data, int len)
{
    {
        std::lock_guard<std::mutex> lock(this->m_writeMtx);
        this->active_bufferPtr->push_back(data);

        /**
         * 问题：
         * 有个bug 如果在添加过程中出现FATAL, 那么应当立即交换缓冲区指向并进行写入, 但是两个缓冲区可能还没有交换,
         * 那么在缓冲区中的数据就无法输出到文件当中，导致最终的报错的日志信息没有被记录
         *
         * 解决：
         * append函数是需要被注册到Logger当中，向AsyncLogging类内的缓冲区中写入格式化之后的日志信息，
         * 所以此时的data实际上已经可以获取到日志级别信息, 通过检测FATAL字符串进行判断
         */

        if ((!this->isFatal(data)) && (this->active_bufferPtr->size() > this->MAX_BUFFER_SIZE))
        {
            this->SwapBufferPtr();
            this->m_cv.notify_one();
        }
        else if (this->isFatal(data))
        {
            this->SwapBufferPtr();
            this->m_cv.notify_one();
        }
    }
    // Stop方法不可以放在里面 否则会发生死锁
    if (this->isFatal(data))
    {
        this->Stop();
        std::cout << "AsyncLogging::append encounter fatal level msg" << std::endl;
        std::abort();
    }
}

void AsyncLogging::flush()
{
    this->m_logfile.flush();
}

void AsyncLogging::AsyncLoggingFunc()
{
    while (this->m_running)
    {
        {
            std::unique_lock<std::mutex> lock(this->m_writeMtx);
            this->m_cv.wait(lock, [this]()
                            { return (!this->standby_bufferPtr->empty()) || (!this->m_running); });
        }
        while (!this->standby_bufferPtr->empty())
        {
            std::string msg_format = this->standby_bufferPtr->front();
            this->m_logfile.append(msg_format.c_str(), msg_format.size());
            this->standby_bufferPtr->pop_front();
        }
        this->flush();
    }
}

void AsyncLogging::SwapBufferPtr()
{
    std::swap(this->active_bufferPtr, this->standby_bufferPtr);
}

bool AsyncLogging::isFatal(const char *data)
{
    // 不区分大小写的查找
    if (strcasestr(data, "fatal") != nullptr)
        return true;
    else
        return false;
}

void AsyncLogging::Stop()
{
    this->m_cv.notify_one(); // 先唤醒
    this->m_running = false; // 再设置运行标志位
    if (this->m_asyncThread.joinable())
        this->m_asyncThread.join();
}
