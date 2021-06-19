#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H

#include<QStringList>
#include<QMutex>
#include<windows.h>

extern QMutex mutex;
extern bool EscFlag;
extern QStringList action_list ;
extern HWND handle;
#endif // GLOBAL_DATA_H
