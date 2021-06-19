#include "mouse_recorded_thread.h"
#include<QDebug>

mouse_recorded_thread::mouse_recorded_thread()
{

}


HHOOK g_hMouse;

LRESULT CALLBACK g_hMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(EscFlag){PostQuitMessage(0);Sleep(100);/*qDebug()<<"mouse recorder exit!";*/}
    if (nCode >= 0)
    {
        PMSLLHOOKSTRUCT pmll = (PMSLLHOOKSTRUCT)lParam;
        if (wParam== WM_LBUTTONDOWN||wParam==WM_RBUTTONDOWN)//||wParam==WM_LBUTTONUP   ||wParam==WM_RBUTTONUP
        {
            POINT a; a.x=pmll->pt.x; a.y=pmll->pt.y; //鼠标点击位置的全局坐标
            HWND H = WindowFromPoint(a);

            qDebug()<<"得到的句柄："<<H<<"   十六进制："<<util::getHex((int)H);
            qDebug()<<"IsWindowVisible判断:"<<IsWindowVisible(H);
            bool bVisible = (::GetWindowLong(H, GWL_STYLE) & WS_VISIBLE) != 0;
            qDebug()<<"GetWindowLong判断："<<bVisible;
            HMENU hMenu;
            hMenu = GetMenu(handle);
            qDebug()<<"菜单："<<hMenu<<"------------------------------------------";
           // CheckMenuItem()
            /*得到窗口标题*/
            char *CStr = "Select Color";size_t len = strlen(CStr) + 1;size_t converted = 0;wchar_t *WStr;WStr=(wchar_t*)malloc(len*sizeof(wchar_t));mbstowcs_s(&converted, WStr, len, CStr, _TRUNCATE);
            ::GetWindowText(H,WStr,128);
            len = wcslen(WStr) + 1;converted = 0;CStr=(char*)malloc(len*sizeof(char));wcstombs_s(&converted, CStr, len, WStr, _TRUNCATE);
            RECT r1;  ::GetWindowRect(H,&r1);
           // qDebug()<<"窗口位置："<<r1.left<<","<<r1.top;
           // qDebug()<<H;
            QString action = "M;"+QString::number((int)H)+";"+QString::number((int)wParam)+";"+QString::number((int)pmll->time)+";"
                    +QString::number(a.x)+";"+QString::number(a.y)+";"+QString(CStr)+";"+QString::number(a.x-r1.left+((a.y-r1.top)<<16));
           //  qDebug()<<action;
            mutex.lock();
            action_list.append(action);
            mutex.unlock();
        }
        if (wParam == WM_MBUTTONDOWN)
             PostQuitMessage(0);


    }

    return CallNextHookEx(g_hMouse, nCode, wParam, lParam);
}



void mousepro() {

    MSG msg;
    g_hMouse = SetWindowsHookEx(WH_MOUSE_LL, g_hMouseProc, GetModuleHandle(NULL), 0);

    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(g_hMouse);
    return;

}


void mouse_recorded_thread::run(){
     mousepro();
}




