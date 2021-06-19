# Auto
A tool that can replay our keyboard/mouse action.



### 前言

本程序是基于Windows平台而构建的一个脚本程序，其制作过程很类似于制作某些小游戏外挂的过程，主要使用了Windows的一众API，如SetWindowsHookEx、PostMessage/SendMessage、WindowFromPoint等等。其中还涉及HANDLE(句柄），HOOK(钩子），Message队列等概念。



### 程序的编写思路

1. 通过建立全局钩子(HOOK)记录键鼠消息

2. 通过Windows API如PostMessage向目标窗口发送捕获到的消息
3. 将捕获到的消息以表格的方式呈现
4. 将消息储存在脚本文件中



### 程序界面

![UI.jpg](https://i.loli.net/2021/06/19/ZneDBQ4FREp8NIm.jpg)

