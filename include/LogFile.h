#ifndef LOGFILE_H
#define LOGFILE_H

#include "FileUtility.h"
#include <memory>
#include <chrono>
#include <ctime>
#include <string>
#include <mutex>

/// @brief ��־�ļ�������
/// ������־�ļ��Ĵ�����д�롢������ˢ��
/// �ײ��ǵ���FileUtility��ķ���
class LogFile
{
    using system_clock_t = std::chrono::system_clock;
    using time_point_t = std::chrono::system_clock::time_point;
    using duration_t = std::chrono::system_clock::duration;

public:
    /// @brief ���캯��
    /// @param filename ��־·��
    /// @param filesize ��־�ļ��޶���С ������ֵ����ҪҪ������־
    /// @param interval ��־�ļ�ˢ��ʱ���� �ײ����FileUtility��fflush�������������е�����д�뵽�ļ���
    /// @param write_num ��־�ļ�д������޶�ֵ
    LogFile(const char *logdir, int filesize, int interval = 3, int write_num = 1024);

    ~LogFile();

    /// @brief ������д�뵽FileUtility�Ļ�������
    /// @param data ��д������
    /// @param len ��д�����ݳ���
    void append(const char *data, int len);

    /// @brief ˢ�»����� ����FileUtility�е�ˢ�·���
    void flush();

    /// @brief �����ļ���С��д��������й�����־
    /// �����и�bug ����־��Ϣ���쵼���´������ļ����ƺ�ԭ������־�ļ�һ��ʱ�ͻ���ԭ������־�ļ��м���д ���������´����ļ�
    void RollFile();

private:
    /// @brief ��ȡ�ļ�����
    /// @return �ļ����ַ���
    std::string GetFileName();

    /// @brief ��ȡ��ǰʱ�������һʱ��
    /// @param now ��ǰ��ʱ���
    /// @param other ����ʱ���
    /// @return ��������ʱ���֮�������
    int GetInterval(LogFile::time_point_t now, LogFile::time_point_t other);

private:
    std::string m_logdir;   // ��־·��
    std::string m_filename; // ��־���� ��ʽ y-m-d h:m:s.log

    off_t m_filesize;                  // д����ļ���С ����ָ����С֮�������־����
    int m_flushInterval;               // ˢ��ʱ����
    int m_writeNumLimit;                    // �ļ�д���������
    int m_count;                       // ��¼�ļ�д�����
    LogFile::time_point_t m_lastRoll;  // �ϴι�����ʱ��� Ҳ�����ϴδ�������־�ļ���ʱ���
    LogFile::time_point_t m_lastFlush; // �ϴ�ˢ�µ�ʱ��

    std::mutex m_appendMtx;              // д����������
    std::unique_ptr<FileUtility> m_file; // �ļ�����������ָ��
};

#endif // LOGFILE_H