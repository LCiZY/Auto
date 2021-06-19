#ifndef MOUSE_RECORDED_THREAD_H
#define MOUSE_RECORDED_THREAD_H
#include<QThread>
#include<QString>
#include<windows.h>
#include<winuser.h>
#include<global_data.h>
#include<util.h>

class mouse_recorded_thread:public QThread
{
public:
    mouse_recorded_thread();
    void run() override;
};

#endif // MOUSE_RECORDED_THREAD_H
