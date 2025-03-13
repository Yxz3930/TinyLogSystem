#include "LogStream.h"

std::string LogStream::str()
{
    return this->oss.str();
}

void LogStream::clear()
{
    this->oss.str("");
    this->oss.clear();
}
