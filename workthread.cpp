#include<workthread.h>
#include<windows.h>

workthread::workthread()
{

}


HHOOK h_kebord;
int flag_exit=0;

LRESULT CALLBACK KebordProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode != HC_ACTION) // 不应该做处理
            return CallNextHookEx(h_kebord, nCode, wParam, lParam);

    PKBDLLHOOKSTRUCT param = (PKBDLLHOOKSTRUCT)lParam;
        if (param->vkCode == VK_F8)
        {
           // PostThreadMessageA(GetCurrentThreadId,WM_QUIT,0,0);
                flag_exit=1;
            PostQuitMessage(0);


        }




    return CallNextHookEx(h_kebord, nCode, wParam, lParam);
}


void workthread::run()
{

    MSG msg2;
    h_kebord=SetWindowsHookEx(WH_KEYBOARD_LL, KebordProc, GetModuleHandle(NULL), 0);
    int bett=0;
    while(bett=GetMessage(&msg2, NULL, 0, 0)!=0)
     {
         if(bett==-1)  PostQuitMessage(0);

         TranslateMessage(&msg2);
         DispatchMessage(&msg2);

     }
     UnhookWindowsHookEx(h_kebord);
}

