#include "FileUtility.h"
#include <string.h>
#include <iostream>

FileUtility::FileUtility(const char *filename)
    : m_writenBytes(0)
{
    this->m_file = fopen(filename, "a");
    if(ferror(this->m_file))
        perror("file open fail");
    ::setbuffer(this->m_file, this->m_buffer, sizeof(this->m_buffer));
}

FileUtility::~FileUtility()
{
    if (this->m_file)
        fclose(this->m_file);
}

void FileUtility::append(const char *data, int len)
{
    size_t writen_bytes = 0;
    size_t remain = len - writen_bytes;
    while (writen_bytes < len)
    {
        off_t ret_len = this->write(data + writen_bytes, remain);
        if (ret_len != remain)
        {
            if (ferror(this->m_file))
            {
                ::perror("FileUtility::append fwrite_unlocked error\n");
                clearerr(this->m_file); // 清楚文件指针的错误标志
            }
            break;
        }
        writen_bytes += ret_len;
    }
    this->m_writenBytes += writen_bytes;
}

void FileUtility::flush()
{
    if (this->m_file)
        ::fflush(this->m_file);
}

off_t FileUtility::WritenBytes()
{
    return this->m_writenBytes;
}

size_t FileUtility::write(const char *data, int len)
{
    size_t ret_len = ::fwrite_unlocked(data, 1, len, this->m_file);
    return ret_len;
}
