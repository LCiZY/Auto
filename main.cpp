//#include<thread>
//#include<process.h>
#include<QDebug>
#include "AuTo.h"
#include <QApplication>
#include<replay_thread.h>






int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    AuTo w;
    w.show();


    return a.exec();
}



