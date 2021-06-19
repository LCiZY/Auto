#ifndef KEYBOARD_RECORDED_THREAD_H
#define KEYBOARD_RECORDED_THREAD_H
#include<QThread>
#include<QString>
#include<windows.h>
#include<util.h>
#include<global_data.h>


class keyboard_recorded_thread:public QThread
{
public:
    keyboard_recorded_thread();
    void run() override;

};

#endif // KEYBOARD_RECORDED_THREAD_H
