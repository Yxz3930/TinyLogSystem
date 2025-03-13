#ifndef LOGSTREAM_H
#define LOGSTREAM_H

#include <string>
#include <sstream>

/// @brief 日志流 重载<<运算符
/// 封装了ostringstream 用于将不同的数据写入字符串缓冲流中
class LogStream
{

public:
    LogStream() = default;

    ~LogStream() = default;

    
    /// @brief 对该类重载<<运算符
    /// @tparam T 模板参数
    /// @param value 不同类型的数据
    /// @return *this 支持链式运算
    template <typename T>
    LogStream &operator<<(const T &value)
    {
        this->oss << value;
        return *this;
    }

    /// @brief 获取读取的字符串
    /// @return 字符串
    std::string str();

    /// @brief 清空字符串缓冲区数据
    void clear();

private:
    std::string buffer;

    std::ostringstream oss;
};




#endif // LOGSTREAM_H