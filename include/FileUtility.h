#ifndef FILEUTILITY_H
#define FILEUTILITY_H

#include <sys/types.h>
#include <cstdio>

/// @brief �ļ������� ���ڴ����ļ���д�롢ˢ�²���
/// ���з�װ���ļ�д�롢������ˢ�µķ���
class FileUtility
{
public:
    /// @brief ���캯��
    /// @param filename Ҫ�򿪵��ļ�·��
    FileUtility(const char *filename);

    /// @brief ��������
    ~FileUtility();

    /// @brief �����ڵ�������׷������ ����ֱ��ͨ����������
    /// @param data ��Ҫд�������
    /// @param len ���ݳ���
    void append(const char *data, int len);

    /// @brief ˢ�»�����
    /// ����fflush֮��ͻὫ���ڻ������е�����д�뵽�ļ�����
    void flush();

    /// @brief ��ȡ�Ѿ����ļ���д��Ĵ�С
    /// @return 
    off_t WritenBytes();

private:
    /// @brief ���ļ���д������
    /// @param data ��д�������
    /// @param len ��д�����ݵĳ���
    /// @return ����ʵ��д��ĳ���
    size_t write(const char *data, int len);

    char m_buffer[64 * 1024]; // �ļ������Ļ���������СΪ64KB���������д��Ч��
    FILE *m_file;             // �ļ�ָ�룬���ڲ����ļ�

    off_t m_writenBytes; // m_buffer���Ѿ�д��Ĵ�С
};

#endif // FILEUTILITY_H