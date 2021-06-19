#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include<QThread>

class workthread: public QThread
{
    Q_OBJECT

public:
    workthread();

protected:

void run();



};

extern int flag_exit;

#endif // WORKTHREAD_H
