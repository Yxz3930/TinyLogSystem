#include <iostream>
#include "LogFile.h"
#include <unistd.h>
#include "LogStream.h"
#include <iomanip>
#include "Logger.h"
#include "AsyncLogging.h"
#include <functional>

void LogFileTest()
{
    LogFile log_file("./logs", 1 * 128, 3, 1024);

    for (int i = 0; i < 100; i++)
    {
        std::string msg = "hello world\n";
        log_file.append(msg.c_str(), msg.size());
        if (i % 10 == 0)
            usleep(200000);
    }
}

void LogStreamTest()
{
    LogStream ls;
    ls << "hello world " << 5;
    std::cout << ls.str() << std::endl;
    ls.clear();

    auto now = std::chrono::system_clock::now();
    time_t time = std::chrono::system_clock::to_time_t(now);
    ls << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    std::cout << ls.str() << std::endl;
}

void LoggerTest()
{
    Logger::SetWritFunc({});
    Logger::SetFlushFunc({});
    LOG_DEBUG << "log debug";
    LOG_INFO << "log info";
    LOG_WARNING << "log warning";
    LOG_ERROR << "log error";
    LOG_FATAL << "log fatal";
}

AsyncLogging async_log("./logs", 1 * 1024, 3, 10);
void write(const char *data, int len)
{
    async_log.append(data, len);
}
void flush()
{
    async_log.flush();
}
void AsyncLoggerTest()
{
    Logger::SetWritFunc([](const char *msg, int len)
                        { async_log.append(msg, len); });
    Logger::SetFlushFunc([]()
                         { async_log.flush(); });

    LOG_DEBUG << "log debug";
    LOG_INFO << "log info";
    LOG_WARNING << "log warning";
    LOG_ERROR << "log error";
    LOG_FATAL << "log fatal";
}

int main()
{
    AsyncLoggerTest();
    return 0;
}