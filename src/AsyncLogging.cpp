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
         * ���⣺
         * �и�bug �������ӹ����г���FATAL, ��ôӦ����������������ָ�򲢽���д��, �����������������ܻ�û�н���,
         * ��ô�ڻ������е����ݾ��޷�������ļ����У��������յı������־��Ϣû�б���¼
         *
         * �����
         * append��������Ҫ��ע�ᵽLogger���У���AsyncLogging���ڵĻ�������д���ʽ��֮�����־��Ϣ��
         * ���Դ�ʱ��dataʵ�����Ѿ����Ի�ȡ����־������Ϣ, ͨ�����FATAL�ַ��������ж�
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
    // Stop���������Է������� ����ᷢ������
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
    // �����ִ�Сд�Ĳ���
    if (strcasestr(data, "fatal") != nullptr)
        return true;
    else
        return false;
}

void AsyncLogging::Stop()
{
    this->m_cv.notify_one(); // �Ȼ���
    this->m_running = false; // ���������б�־λ
    if (this->m_asyncThread.joinable())
        this->m_asyncThread.join();
}
