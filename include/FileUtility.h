#ifndef FILEUTILITY_H
#define FILEUTILITY_H

#include <sys/types.h>
#include <cstdio>

/// @brief 文件工具类 用于处理文件的写入、刷新操作
/// 类中封装了文件写入、缓冲区刷新的方法
class FileUtility
{
public:
    /// @brief 构造函数
    /// @param filename 要打开的文件路径
    FileUtility(const char *filename);

    /// @brief 析构函数
    ~FileUtility();

    /// @brief 向类内的数组中追加数据 并且直接通过缓冲区向
    /// @param data 需要写入的数据
    /// @param len 数据长度
    void append(const char *data, int len);

    /// @brief 刷新缓冲区
    /// 调用fflush之后就会将类内缓冲区中的数据写入到文件当中
    void flush();

    /// @brief 获取已经向文件中写入的大小
    /// @return 
    off_t WritenBytes();

private:
    /// @brief 向文件中写入数据
    /// @param data 待写入的数据
    /// @param len 待写入数据的长度
    /// @return 返回实际写入的长度
    size_t write(const char *data, int len);

    char m_buffer[64 * 1024]; // 文件操作的缓冲区，大小为64KB，用于提高写入效率
    FILE *m_file;             // 文件指针，用于操作文件

    off_t m_writenBytes; // m_buffer中已经写入的大小
};

#endif // FILEUTILITY_H