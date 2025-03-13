#ifndef LOGSTREAM_H
#define LOGSTREAM_H

#include <string>
#include <sstream>

/// @brief ��־�� ����<<�����
/// ��װ��ostringstream ���ڽ���ͬ������д���ַ�����������
class LogStream
{

public:
    LogStream() = default;

    ~LogStream() = default;

    
    /// @brief �Ը�������<<�����
    /// @tparam T ģ�����
    /// @param value ��ͬ���͵�����
    /// @return *this ֧����ʽ����
    template <typename T>
    LogStream &operator<<(const T &value)
    {
        this->oss << value;
        return *this;
    }

    /// @brief ��ȡ��ȡ���ַ���
    /// @return �ַ���
    std::string str();

    /// @brief ����ַ�������������
    void clear();

private:
    std::string buffer;

    std::ostringstream oss;
};




#endif // LOGSTREAM_H