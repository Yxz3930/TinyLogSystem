#include <iostream>
#include "Logger.h"
#include <iomanip>

// 静态成员 类内声明 类外定义
std::function<void(const char *, int)> Logger::m_write_cb = nullptr;
std::function<void()> Logger::m_flush_cb = nullptr;

// static LogFile log_file("./logs", 1 * 1024);

static void DefaultWriteFunc(const char *data, int len)
{
    // log_file.append(data, len);   // 输出到文件
    fwrite(data, 1, len, stdout); // 输出到终端
}

static void DefaultFlushFunc()
{
    // log_file.flush();                     // 文件刷新缓冲区
    std::cout << std::flush << std::endl; // 终端刷新缓冲区
}

Logger::Formatter::Formatter(LogLevel level, const char *file, int line)
    : m_level(level), m_file(file), m_line(line)
{
}

std::string Logger::Formatter::MsgtoFormat(const char *msg)
{
    std::string filename = this->GetCodeFileName();
    auto now = std::chrono::system_clock::now();
    time_t time = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;

    std::string level_str = this->LeveltoStr(this->m_level);
    std::string filename_line = std::string(" [") + filename + ":" + std::to_string(this->m_line) + "]";
    oss << "[" << std::put_time(::localtime(&time), "%Y-%m-%d %H:%M:%S") << "] "
        << "[" << level_str << "]" << "\t"
        << std::setw(20) << std::left << filename_line << "\t"
        << "msg: " << msg << "\n";

    return oss.str();
}

std::string Logger::Formatter::GetCodeFileName()
{
    ssize_t pos = this->m_file.find_last_of("/");
    return this->m_file.substr(pos + 1);
}

std::string Logger::Formatter::LeveltoStr(LogLevel level)
{
    switch (level)
    {
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARNING:
        return "WARNING";
    case LogLevel::ERROR:
        return "ERROR";
    case LogLevel::FATAL:
        return "FATAL";
    }
    return "UNKNOWN";
}

Logger::Logger(LogLevel level, const char *__file__, int __line__)
    : m_formatter(level, __file__, __line__)
{
}

Logger::~Logger()
{
    std::string msg = this->m_logstream.str();
    std::string msg_format = this->m_formatter.MsgtoFormat(msg.c_str());
    this->m_write_cb(msg_format.c_str(), msg_format.size());

    if (this->m_formatter.m_level == LogLevel::FATAL)
    {
        this->m_flush_cb();
        std::cout << "Logger::~Logger encounter fatal level msg" << std::endl;
        std::abort();
    }
}

void Logger::SetWritFunc(std::function<void(const char *msg, int len)> cb)
{
    if (cb)
        Logger::m_write_cb = cb;
    else
        Logger::m_write_cb = DefaultWriteFunc;
}

void Logger::SetFlushFunc(std::function<void(void)> cb)
{
    if (cb)
        Logger::m_flush_cb = cb;
    else
        Logger::m_flush_cb = DefaultFlushFunc;
}
