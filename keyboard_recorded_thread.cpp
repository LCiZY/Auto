#include "keyboard_recorded_thread.h"

#include<QDebug>
#include<QApplication>
keyboard_recorded_thread::keyboard_recorded_thread()
{

}



HHOOK g_hKebord;
BOOL g_bCapsLock = FALSE;
BOOL g_bShift = FALSE;
BOOL FIRSTTAP=TRUE,IFCAPSLOCK;
HKL  hCurKL;
LRESULT CALLBACK g_hKebordProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    g_bCapsLock  = GetKeyState(VK_CAPITAL);  //大写为1
    g_bShift = GetAsyncKeyState(VK_LSHIFT)& 0x8000;  //
    if(FIRSTTAP){
        FIRSTTAP=FALSE;
        IFCAPSLOCK=g_bCapsLock;/*记录大小写*/
       // LoadKeyboardLayout((LPCWSTR)QString("0x0409").utf16(), KLF_ACTIVATE);
        hCurKL = GetKeyboardLayout(0);/*记录输入法*/
    }//qDebug()<<"大小写开始是:"<<g_bCapsLock;
    if (nCode >= 0)
    {

            PKBDLLHOOKSTRUCT pmll = (PKBDLLHOOKSTRUCT)lParam;

            if (pmll->vkCode==VK_ESCAPE){
               EscFlag=true;
               //qDebug()<<"大小写最后是:"<<g_bCapsLock;
               if(IFCAPSLOCK^g_bCapsLock){keybd_event(VK_CAPITAL,0 ,0, 0);   keybd_event(VK_CAPITAL,0,KEYEVENTF_KEYUP, 0);  } //大小写复位 qDebug()<<"大小写复位:";
                                          ActivateKeyboardLayout(hCurKL, 0);     //shift复位
               PostQuitMessage(0); Sleep(100); //qDebug()<<"keybord recorder exit!";
            }

            DWORD C;
            if(pmll->vkCode >= 65 && pmll->vkCode <= 90)  //如果输入的是26个字母
                C=g_bCapsLock^g_bShift?pmll->vkCode:(pmll->vkCode+32);
            else
                C=pmll->vkCode;
            //wParam==256  //如果是按键【按下】
            QString action ="K;"+ QString::number((int)C) +";"+ (wParam==256?"0":"2") +";"+ QString::number((int)pmll->time);
            //qDebug()<<action; qDebug()<<"g_bShift:"<<g_bShift;

            mutex.lock();
            action_list.append(action);
            mutex.unlock();




    }

    return CallNextHookEx(g_hKebord, nCode, wParam, lParam);
}



void keyboardpro() {

    MSG msg;
    g_hKebord = SetWindowsHookEx(WH_KEYBOARD_LL, g_hKebordProc, GetModuleHandle(NULL), 0);

    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(g_hKebord);
    return;

}


void keyboard_recorded_thread::run(){
     keyboardpro();
}


