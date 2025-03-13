#ifndef ASYNCLOGGING_H
#define ASYNCLOGGING_H

#include <string>
#include "LogFile.h"
#include <deque>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <thread>
#include <atomic>

/// @brief �첽��־��
/// ���Ǹ���ֻ�����첽��־����������ʵ�֣�����˫���塢�����߳�д�����ݣ���󷽷���Ҫע�ᵽLogger����
/// Ϊ�˼򵥷��� ʹ�����е�������Ϊ������б���
class AsyncLogging
{
    using Buffer_t = std::deque<std::string>;
    using BufferPtr_t = std::unique_ptr<std::deque<std::string>>;

public:
    /// @brief ���캯�� ��������LogFile��Ĳ���
    /// @param logdir ��־�ļ�·��
    /// @param filesize ��־�ļ���С�޶�
    /// @param interval ��־ˢ��ʱ����
    /// @param write_num ��־�ļ�д������޶�
    AsyncLogging(const char *logdir, int filesize, int interval = 3, int write_num = 1024);

    /// @brief ��������
    ~AsyncLogging();

    /// @brief �첽��־��ǰ�������־��Ϣ��������
    /// append�����ᱻע�ᵽLogger����m_write_cb�ص������� Ҳ����˵�����data���ǻ������־������Ϣ��
    /// @param data ��־��Ϣ
    /// @param len ��־��Ϣ����
    void append(const char *data, int len);

    /// @brief ˢ���ļ�������
    void flush();

    /// @brief �첽�̹߳�������
    void AsyncLoggingFunc();

private:
    /// @brief ����������������ָ��
    void SwapBufferPtr();

    /// @brief �ж���־��Ϣ���Ƿ����FATAL
    /// @param data ��ʽ��֮�����־��Ϣ
    /// @param len ��־��Ϣ����
    bool isFatal(const char* data);

    /// @brief ֹͣ�첽��־�߳�
    void Stop();

private:
    LogFile m_logfile; // �����߳�д���ļ�����

    // �̡߳�����ر���
    std::thread m_asyncThread;    // ����첽д����߳�
    std::mutex m_writeMtx;        // д�����������
    std::condition_variable m_cv; // ��������
    std::atomic_bool m_running{false}; // ���б�־

    // ��������س�Ա����
    std::deque<std::string> buffer_1, buffer_2;
    BufferPtr_t active_bufferPtr;  // ָ��ǰ��д��Ļ�����
    BufferPtr_t standby_bufferPtr; // ָ�����߳�д���ļ��Ļ�����
    int MAX_BUFFER_SIZE;           // ��������С�޶�
};

#endif // ASYNCLOGGING_H