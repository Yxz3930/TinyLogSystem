#include "LogFile.h"
#include <iomanip>
#include <sstream>
#include <iostream>

LogFile::LogFile(const char *logdir, int filesize, int interval, int write_num)
    : m_logdir(logdir), m_filesize(filesize), m_flushInterval(interval), m_writeNumLimit(write_num), m_count(0),
      m_lastFlush(system_clock_t::now()), m_lastRoll(system_clock_t::now()), m_filename("xxx.log")
{
    this->RollFile();
}

LogFile::~LogFile()
{
}

void LogFile::append(const char *data, int len)
{
    std::lock_guard<std::mutex> lock(this->m_appendMtx);
    this->m_file->append(data, len);
    ++this->m_count;

    // 按写入文件大小滚动
    // std::cout << "writen bytes: " << this->m_file->WritenBytes() << std::endl;
    // std::cout << "file size: " << this->m_filesize << std::endl;
    if (this->m_file->WritenBytes() >= this->m_filesize)
    {
        // std::cout << "write bytes complete" << std::endl;
        this->RollFile();
        return;
    }
    // 按写入次数进行滚动
    if (this->m_count >= this->m_writeNumLimit)
    {
        // std::cout << "write count complete" << std::endl;
        this->RollFile();
        return;
    }

    // 再来判断下是否需要刷新
    if (this->GetInterval(system_clock_t::now(), this->m_lastFlush) > this->m_flushInterval)
    {
        this->m_lastFlush = system_clock_t::now();
        this->flush();
    }
}

void LogFile::flush()
{
    this->m_file->flush();
}

void LogFile::RollFile()
{
    std::string filename = this->GetFileName();
    time_point_t now = system_clock_t::now();
    if (now > this->m_lastRoll)
    {
        this->m_count = 0;
        this->m_lastFlush = now;
        this->m_lastRoll = now;
        std::cout << "new file name: " << filename << std::endl;
        this->m_file.reset(new FileUtility(filename.c_str()));
    }
}

std::string LogFile::GetFileName()
{
    LogFile::time_point_t now = system_clock_t::now();
    struct tm tm;
    time_t time;
    time = std::chrono::system_clock::to_time_t(now);
    localtime_r(&time, &tm);

    std::ostringstream oss;
    oss << this->m_logdir << "/";
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << ".log";
    return oss.str();
}

int LogFile::GetInterval(LogFile::time_point_t now, LogFile::time_point_t other)
{
    duration_t duration = now - other;
    int seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    return seconds;
}
