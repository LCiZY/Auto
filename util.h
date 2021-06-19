#ifndef UTIL_H
#define UTIL_H

#include<windows.h>
#include<QString>
#include<QTime>

class util
{
public:
    util();
    static LPCWSTR char2wchar(const char *szStr);
    static QString getMinuteSecond();
    static QString getHex(int source);
    static QString getHex(QString source);
};

#endif // UTIL_H
