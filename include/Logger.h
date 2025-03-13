#ifndef LOGGER_H
#define LOGGER_H

#include "LogFile.h"
#include "LogStream.h"
#include <functional>

enum LogLevel
{
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class Logger
{

public:
    /// @brief 构造函数 并初始化类中的成员变量
    /// @param level 日志级别
    /// @param __file__ 文件名
    /// @param __line__ 代码行
    Logger(LogLevel level, const char *__file__, int __line__);

    /// @brief 析构函数
    /// 在析构函数中获取字符串缓冲区中的数据 然后进行写入
    ~Logger();

    /// @brief 返回LogStream日志流对象
    /// @return
    LogStream &stream() { return this->m_logstream; };

    /// @brief 注册写操作回调函数
    /// @param
    static void SetWritFunc(std::function<void(const char *msg, int len)>);

    /// @brief 注册刷新缓冲区回调函数
    /// @param
    static void SetFlushFunc(std::function<void(void)>);

private:
    static std::function<void(const char *msg, int len)> m_write_cb;
    static std::function<void(void)> m_flush_cb;

    LogStream m_logstream;

private:
    /// @brief 用于格式化日志信息
    class Formatter
    {
    public:
        /// @brief Formatter构造
        /// @param level
        /// @param file
        /// @param line
        Formatter(LogLevel level, const char *file, int line);

        /// @brief 将日志信息格式化
        std::string MsgtoFormat(const char *msg);

        /// @brief 获取代码文件名 去掉其中的路径
        /// @return 文件名
        std::string GetCodeFileName();

        /// @brief 将日志级别转换为字符串
        /// @param level 日志级别
        /// @return 字符串
        std::string LeveltoStr(LogLevel level);

        LogLevel m_level;
        std::string m_file;
        int m_line;
    };

private:
    Formatter m_formatter; // 这里需要进行实例化 否则会报错
};


#endif // LOGGER_H

#define LOG_DEBUG Logger(LogLevel::DEBUG, __FILE__, __LINE__).stream()
#define LOG_INFO Logger(LogLevel::INFO, __FILE__, __LINE__).stream()
#define LOG_WARNING Logger(LogLevel::WARNING, __FILE__, __LINE__).stream()
#define LOG_ERROR Logger(LogLevel::ERROR, __FILE__, __LINE__).stream()
#define LOG_FATAL Logger(LogLevel::FATAL, __FILE__, __LINE__).stream()
