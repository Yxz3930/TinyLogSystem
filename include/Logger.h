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
    /// @brief ���캯�� ����ʼ�����еĳ�Ա����
    /// @param level ��־����
    /// @param __file__ �ļ���
    /// @param __line__ ������
    Logger(LogLevel level, const char *__file__, int __line__);

    /// @brief ��������
    /// �����������л�ȡ�ַ����������е����� Ȼ�����д��
    ~Logger();

    /// @brief ����LogStream��־������
    /// @return
    LogStream &stream() { return this->m_logstream; };

    /// @brief ע��д�����ص�����
    /// @param
    static void SetWritFunc(std::function<void(const char *msg, int len)>);

    /// @brief ע��ˢ�»������ص�����
    /// @param
    static void SetFlushFunc(std::function<void(void)>);

private:
    static std::function<void(const char *msg, int len)> m_write_cb;
    static std::function<void(void)> m_flush_cb;

    LogStream m_logstream;

private:
    /// @brief ���ڸ�ʽ����־��Ϣ
    class Formatter
    {
    public:
        /// @brief Formatter����
        /// @param level
        /// @param file
        /// @param line
        Formatter(LogLevel level, const char *file, int line);

        /// @brief ����־��Ϣ��ʽ��
        std::string MsgtoFormat(const char *msg);

        /// @brief ��ȡ�����ļ��� ȥ�����е�·��
        /// @return �ļ���
        std::string GetCodeFileName();

        /// @brief ����־����ת��Ϊ�ַ���
        /// @param level ��־����
        /// @return �ַ���
        std::string LeveltoStr(LogLevel level);

        LogLevel m_level;
        std::string m_file;
        int m_line;
    };

private:
    Formatter m_formatter; // ������Ҫ����ʵ���� ����ᱨ��
};


#endif // LOGGER_H

#define LOG_DEBUG Logger(LogLevel::DEBUG, __FILE__, __LINE__).stream()
#define LOG_INFO Logger(LogLevel::INFO, __FILE__, __LINE__).stream()
#define LOG_WARNING Logger(LogLevel::WARNING, __FILE__, __LINE__).stream()
#define LOG_ERROR Logger(LogLevel::ERROR, __FILE__, __LINE__).stream()
#define LOG_FATAL Logger(LogLevel::FATAL, __FILE__, __LINE__).stream()
