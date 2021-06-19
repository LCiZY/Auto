#ifndef REPLAY_THREAD_H
#define REPLAY_THREAD_H

#include<QThread>
#include<QString>
#include<stdlib.h>
#include<windows.h>
#include<global_data.h>
#include<QDebug>

class replay_thread : public QThread
{
    Q_OBJECT
public:
    replay_thread();
    /*replay_time：回放次数  start_delay：每次开始回放前等待的秒数*/
    replay_thread(int replay_times,int start_delay=2);
    void run() override;
    void setReplayTimes(int times);
    void setStart_delay(int delay);

private:
    int replayTimes;
    int start_delay;
};


#endif // REPLAY_THREAD_H
