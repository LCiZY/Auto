#include "replay_thread.h"
#include<windows.h>
#include<QApplication>
void sendMKMessage(HWND NEWHWND, QString, int, int);
void sendMKMessage(HWND NEWHWND,QString,int);


replay_thread::replay_thread()
{
    replayTimes = 1; start_delay = 2;
}

replay_thread::replay_thread(int replay_times,int start_delay)
{
     this->replayTimes = replay_times>0?replay_times:1;
     this->start_delay = start_delay>0?start_delay:0;
}


void replay_thread::run(){

    for(int i=0;i<replayTimes;i++){

        Sleep(start_delay*1000);

        if(action_list.size()==0)return;
        int lastMsgTime=QString(QStringList(QString(action_list.at(0)).split(";")).at(3)).toInt();
        foreach (QString str, action_list) {
         //   qDebug()<<"本次执行动作："<<str;
            QStringList args = str.split(";"); int now = QString(args[3]).toInt();
            Sleep(now-lastMsgTime);
              int code = QString(args[1]).toInt();
            if(QString::compare(args[0],"K")==0){ //是键盘消息
                if(QString::compare(args[2],"0")==0){//按下按键
     //               keybd_event(code,translateKeyCode(code),0, 0);  //模拟输入按键
                    if(code>=97&&code<=122)
                     { //keybd_event(VK_LSHIFT,0 ,0, 0);
                        keybd_event(code-32,0 ,0, 0);
                       // keybd_event(VK_LSHIFT,0,KEYEVENTF_KEYUP, 0);
                    }
                    else
                      keybd_event(code,0 ,0, 0);
                }else{
                    keybd_event(code,0,KEYEVENTF_KEYUP, 0);  //模拟fangkai按键
                }

            }else if(QString::compare(args[0],"M")==0){//鼠标消息
                HWND NEWHWND= (HWND)code;
                //先看看原来的窗口句柄还在不在
                char *CStr = const_cast<char *>(QString(args[6]).toStdString().c_str());size_t len = strlen(CStr) + 1;size_t converted = 0;wchar_t *WStr;WStr=(wchar_t*)malloc(len*sizeof(wchar_t));mbstowcs_s(&converted, WStr, len, CStr, _TRUNCATE);
                ::GetWindowText(NEWHWND,WStr,128); //根据句柄获取窗口名
                len = wcslen(WStr) + 1;converted = 0;CStr=(char*)malloc(len*sizeof(char));wcstombs_s(&converted, CStr, len, WStr, _TRUNCATE);
                if(strlen(CStr)==0){//窗口名为空，原来的窗口句柄不在了，根据原来的窗口名找一下新句柄
                     CStr = const_cast<char *>(QString(args[6]).toStdString().c_str()); len = strlen(CStr) + 1; converted = 0;WStr=(wchar_t*)malloc(len*sizeof(wchar_t));mbstowcs_s(&converted, WStr, len, CStr, _TRUNCATE);
                   // qDebug()<<"窗口名为空，原来的窗口句柄不在了，根据原来的窗口名找一下新句柄，要找："<<CStr;
                    NEWHWND = ::FindWindow(NULL,WStr);
                   // qDebug()<<"窗口名为空，原来的窗口句柄不在了，根据原来的窗口名找一下新句柄，找到："<<NEWHWND;
                    if((int)NEWHWND==0){//原来标题的窗口都找不着了
                            NEWHWND = ::GetForegroundWindow();
                    //        qDebug()<<"原来的窗口都找不着了，顶层窗口的句柄："<<NEWHWND;
                            RECT r1;  ::GetWindowRect(NEWHWND,&r1);
                           // qDebug()<<"窗口位置："<<r1.left<<","<<r1.top; //全局坐标转相对
                             int xpos=QString(args[4]).toInt()-r1.left; int ypos=QString(args[5]).toInt()-r1.top;
                            sendMKMessage(NEWHWND,args[2],xpos,ypos);
                            continue;
                    }
                }

                 sendMKMessage(NEWHWND,args[2],QString(args[7]).toInt());



            }
            lastMsgTime = now;
        }


    }

}

void sendMKMessage(HWND NEWHWND,QString event,int xpos,int ypos){

    if(event.compare("513")==0){  //鼠标左键
        PostMessage(NEWHWND,WM_LBUTTONDOWN,MK_LBUTTON,xpos+(ypos<<16));
        PostMessage(NEWHWND,WM_LBUTTONUP,0,xpos+(ypos<<16));
    }else if(event.compare("516")==0){  //鼠标右键
        PostMessage(NEWHWND,WM_RBUTTONDOWN,MK_RBUTTON,xpos+(ypos<<16));
        PostMessage(NEWHWND,WM_RBUTTONUP,0,xpos+(ypos<<16));
    }



}


void sendMKMessage(HWND NEWHWND,QString event,int param){

    if(event.compare("513")==0){  //鼠标左键
        PostMessage(NEWHWND,WM_LBUTTONDOWN,MK_LBUTTON,param);
        PostMessage(NEWHWND,WM_LBUTTONUP,0,param);
    }else if(event.compare("516")==0){  //鼠标右键
        PostMessage(NEWHWND,WM_RBUTTONDOWN,MK_RBUTTON,param);
        PostMessage(NEWHWND,WM_RBUTTONUP,0,param);
    }



}


void replay_thread::setReplayTimes(int times){
    this->replayTimes = times;
}

void replay_thread::setStart_delay(int delay){
    this->start_delay = delay;
}
