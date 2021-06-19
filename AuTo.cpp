#include "AuTo.h"
#include "ui_AuTo.h"
#include<iostream>
#include<windows.h>
#include<conio.h>
#include <stdio.h>
#include <QTime>
#include <QThread>
#include<QFile>
#include"QFileDialog"
#include<QMessageBox>
#include<replay_thread.h>
#include<QDebug>
#include<QKeyEvent>

extern bool close;

int translateKeyCode(int key);
struct point{
    int x;
    int y;
};

AuTo::AuTo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AuTo)
{
       ui->setupUi(this);
       QFile qssfile(":/style/style.qss");QString qss; if(qssfile.open(QFile::ReadOnly)){ qss = qssfile.readAll();qssfile.close();}
       this->setWindowIcon(QIcon(":/logo/icon.jpg"));
       this->setStyleSheet(qss);
       this->setWindowTitle(WINDOWSOURCENAME);
       handle = (HWND)this->winId();
       /*遮罩层初始化*/
       mpShadeWindow = new  QWidget(ui->tableWidget);
       QString str("QWidget{background-color:rgba(255,255,255,0.6);}");   //0.6代表透明度，根据需要自行调整
       mpShadeWindow->setStyleSheet(str);
       mpShadeWindow->hide();


       /*“文件”主菜单*/
       fileMenu = new QMenu(tr("文件"));
       openScriptFileAC = new QAction(QIcon(":/icon/openFile_ac_32.png"),tr("打开脚本文件"));openScriptFileAC->setCheckable(false);
       saveScriptFileAC = new QAction(QIcon(":/icon/ac/save_ac_32"),tr("保存"));saveScriptFileAC->setCheckable(false);
       saveAsScriptFileAC = new QAction(QIcon(":/icon/filesaveas_ac_32.png"),tr("脚本另存为"));saveAsScriptFileAC->setCheckable(false);
       exitAC = new QAction(QIcon(":/icon/exit_ac_32.png"),tr("退出")); exitAC->setCheckable(false);
       fileMenu->addAction(openScriptFileAC);fileMenu->addAction(saveScriptFileAC); fileMenu->addAction(saveAsScriptFileAC);fileMenu->addSeparator(); fileMenu->addAction(exitAC);
       ui->menuBar->addMenu(fileMenu);

       /*“录制”主菜单*/
       recordMenu = new QMenu(tr("录制"));
       startRecAC = new QAction(QIcon(":/icon/start_ac_32.png"),tr("开始录制")); startRecAC->setCheckable(false);
       stopRecAC = new QAction(QIcon(":/icon/stop_ac_32.png"),tr("停止录制"));   stopRecAC->setCheckable(false);
       recordMenu->addAction(startRecAC); recordMenu->addAction(stopRecAC);
       ui->menuBar->addMenu(recordMenu);


       /*“回放”主菜单*/
       replayMenu = new QMenu(tr("回放"));
       startReplayAC = new QAction(QIcon(":/icon/start_ac_32.png"),tr("开始回放"));   startReplayAC->setCheckable(false);
       stopReplayAC = new QAction(QIcon(":/icon/stop_ac_32.png"),tr("停止回放"));     stopReplayAC->setCheckable(false);
       replayMenu->addAction(startReplayAC); replayMenu->addAction(stopReplayAC);
       ui->menuBar->addMenu(replayMenu);

       /*“选项”主菜单*/
       configMenu = new QMenu(tr("选项"));
       settingAC = new QAction(QIcon(":/icon/setting_ac_32.png"),tr("设置")); settingAC->setCheckable(false);
       configMenu->addAction(settingAC);
       ui->menuBar->addMenu(configMenu);

       /*“视图”主菜单*/
       viewMenu = new QMenu(tr("视图"));
          /*“脚本视图”子菜单*/
          scriptViewMenu = viewMenu->addMenu(QIcon(":/icon/ac/script_ac.png"),tr("脚本视图"));
          scriptViewACGroup = new QActionGroup(this);
          showScriptViewAC = new QAction(QIcon(":/icon/ac/show_detail_ac_32.png"),tr("显示脚本详情"),scriptViewACGroup);showScriptViewAC->setCheckable(true);
          shutScriptViewAC = new QAction(QIcon(":/icon/ac/shut_detail_ac_32.png"),tr("关闭脚本详情"),scriptViewACGroup);shutScriptViewAC->setCheckable(true);
       scriptViewMenu->addAction(showScriptViewAC);
       scriptViewMenu->addAction(shutScriptViewAC);
       ui->menuBar->addMenu(viewMenu);

       /*状态栏*/
       ui->statusBar->setSizeGripEnabled(false);ui->statusBar->setFont(QFont(tr("微软雅黑")));ui->statusBar->setStyleSheet(tr("background-color:rgb(255,255,255,255);color:rgb(145,145,145,255);"));//去掉状态栏右下角的三角
       //ui->statusBar->showMessage(tr("临时信息!"),2000);//显示临时信息2000ms 前面的正常信息被覆盖 当去掉后一项时，会一直显示
       permanent=new QLabel(this); permanent->setFrameStyle(QFrame::Sunken);permanent->setFont(QFont(tr("Verdana"))); permanent->setText(tr("Started.")+tr("  ")+util::getMinuteSecond());
       ui->statusBar->addPermanentWidget(permanent);//显示永久信息

       /*子窗口*/
       setting_window=new setting;

     connect(openScriptFileAC,SIGNAL(triggered(bool)), this, SLOT(open_script_file()));
     connect(saveScriptFileAC,SIGNAL(triggered(bool)), this, SLOT(script_file_save()));
     connect(saveAsScriptFileAC,SIGNAL(triggered(bool)), this, SLOT(script_file_save_as()));
     connect(exitAC,SIGNAL(triggered(bool)), this, SLOT(exitApp()));
     connect(startRecAC,SIGNAL(triggered(bool)), this, SLOT(recordAction()));
     connect(stopRecAC,SIGNAL(triggered(bool)), this, SLOT(stopRecordAction()));
     connect(startReplayAC,SIGNAL(triggered(bool)), this, SLOT(replay()));
     connect(stopReplayAC,SIGNAL(triggered(bool)), this, SLOT(stop_replay()));
     connect(settingAC,SIGNAL(triggered(bool)), this, SLOT(showSettingWindow()));
     connect(showScriptViewAC,SIGNAL(toggled(bool)),this,SLOT(showScriptDetail(bool)));
     connect(shutScriptViewAC,SIGNAL(toggled(bool)),this,SLOT(shutScriptDetail(bool)));
     connect(setting_window,SIGNAL(argsChange()),this,SLOT(argsChange()));

     QObject::connect(&_keyboard_recorded_thread, SIGNAL(finished()), this, SLOT(recordActionFinished()));
     QObject::connect(&_mouse_recorded_thread, SIGNAL(finished()), this, SLOT(recordActionFinished()));
     QObject::connect(&_replay_thread,SIGNAL(finished()),this,SLOT(replayFinished()));

     //endingmusic.setSource(QUrl::fromLocalFile("complete.wav")); endingmusic.setVolume(0.8f);

     ui->tableWidget->hide();

}


AuTo::~AuTo()
{
    delete ui;

}

void AuTo::showSettingWindow()
{
    ui->statusBar->showMessage(tr("打开设置窗口"),2000);
    setting_window->show();
}


void AuTo::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


void AuTo::recordAction()
{
    disableAllMenu();
    recordMenu->setEnabled(true);
    startRecAC->setEnabled(false);

    permanent->setText(tr("开始录制")+tr("  ")+util::getMinuteSecond());

    action_list.clear();EscFlag=false;
    _keyboard_recorded_thread.start();
    _mouse_recorded_thread.start();


}

void AuTo::stopRecordAction(){
    keybd_event(0x1B,0 ,0, 0);
    recordFinishCount=2;
    recordActionFinished();
}

void AuTo::recordActionFinished()
{
      if(++recordFinishCount%2!=0) return;
      permanent->setText(tr("录制结束")+tr("  ")+util::getMinuteSecond());
      startRecAC->setEnabled(true);
      enableAllMenu();
      setWindownTitleDashFileName(tr("未命名"));
}

bool AuTo::read_actions_from_script_file(){

    QString path = QFileDialog::getOpenFileName(this,tr("选择脚本文件"),".",tr("脚本文件(*.autoscript)"));
    if(path.isEmpty()) return false;
    QFile script(path);
    if(script.open(QIODevice::ReadOnly)){
        action_list.clear();
        char buf[128];
         qint64 lineLength = 0;
        while (lineLength!=-1) {
            lineLength = script.readLine(buf, sizeof(buf));
            if (lineLength == -1) break;
            QString linedata(buf);
            linedata = linedata.trimmed();
           // qDebug()<<"读取行："<<linedata;
            if(linedata.compare("")==0) {
                continue;
            }
            action_list.append(linedata);
        }
        script.close();
        scriptFilePath = path;
        ui->statusBar->showMessage(tr("脚本文件读取完成"),2000);
        return true;
    }else{
              QMessageBox::warning(this,tr("文件打开失败"),tr("文件打开失败，请重试！"));
              return false;
    }
}

void AuTo::open_script_file()
{
    ui->menuBar->setEnabled(false);
    this->setCursor(QCursor(Qt::WaitCursor));

    if(read_actions_from_script_file()) setWindownTitleDashFileName(scriptFilePath);

    this->setCursor(QCursor(Qt::ArrowCursor));
    ui->menuBar->setEnabled(true);
}

void AuTo::script_file_save(){
    if(scriptFilePath.isEmpty()){
        script_file_save_as(); return;
    }

    if(action_list.size()==0) {QMessageBox::about(this,tr("提示"),tr("请先录制脚本或打开脚本文件"));return;}
    if(write_actions_to_script_file(scriptFilePath)) setWindownTitleDashFileName(scriptFilePath);
}


void AuTo::script_file_save_as(){

    if(action_list.size()==0) {QMessageBox::about(this,tr("提示"),tr("请先录制脚本或打开脚本文件"));return;}
    QString path = QFileDialog::getSaveFileName(
                this,
                tr("选择保存位置"),
                tr("./untitled.autoscript"),
                tr("myword files(*.autoscript)")
            );
    if(path.isEmpty()) return;
    if(write_actions_to_script_file(path)) setWindownTitleDashFileName(path);
}

bool AuTo::write_actions_to_script_file(QString script_file_path){
    QString result;
      result.append(action_list.at(0));
    for(int i =1;i<action_list.size();i++){
        result.append("\n").append(action_list.at(i));
    }
    QFile script(script_file_path);
    if(script.open(QIODevice::WriteOnly)){
        script.write(result.toStdString().c_str(),strlen(result.toStdString().c_str()));
        script.close();
        scriptFilePath = script_file_path;
        ui->statusBar->showMessage(tr("保存成功"),2000);
        return true;
    }else{
        QMessageBox::warning(this,tr("保存失败！"),tr("保存失败，请重试！"));
        return false;
    }
}

void AuTo::showScriptDetail(bool flag){
    if(!flag) return;
    if(action_list.isEmpty()) {QMessageBox::about(this,tr("提示"),tr("请先录制脚本或打开脚本文件"));showScriptViewAC->setChecked(false); return;}
    ui->statusBar->showMessage(tr("正在读取脚本文件详情"),2000);
    ui->menuBar->setEnabled(false);
    this->setCursor(QCursor(Qt::WaitCursor));
    mpShadeWindow->setGeometry(ui->tableWidget->rect());   //遮罩窗口位置
    mpShadeWindow->show();

    disconnect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(editScriptDetail(int,int)));
    createScriptDetailList();
    connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(editScriptDetail(int,int)));

    ui->tableWidget->show();
    mpShadeWindow->hide();
    this->setCursor(QCursor(Qt::ArrowCursor));
    ui->menuBar->setEnabled(true);
    ui->statusBar->showMessage(tr("已显示详情"),2000);
}

void AuTo::shutScriptDetail(bool flag){
    if(!flag) return;
    ui->tableWidget->hide();
    ui->statusBar->showMessage(tr("已关闭详情"),2000);
}

void AuTo::createScriptDetailList(){
    if(action_list.isEmpty()) return;

    ui->tableWidget->setRowCount(action_list.size());
    ui->tableWidget->setColumnCount(SCRIPTLISTCOLUMNSNUM);
    ui->tableWidget->setFont(QFont("Microsoft YaHei"));
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //鼠标事件累加顺序：   0134675    //键盘事件累加顺序： 0324
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() <<tr("事件类型")<< tr("窗口句柄")<<tr("击键/释放")<<tr("事件码")<<tr("事件发生时间")<<tr("窗口名")<<tr("X")<<tr("Y"));
    ui->tableWidget->horizontalHeaderItem(2)->setToolTip(tr("0表示按下按键，2表示释放按键"));
    ui->tableWidget->horizontalHeaderItem(3)->setToolTip(tr("对于按键事件，表示虚拟键码；\n对于鼠标事件，则表示鼠标事件代码"));
    ui->tableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    QTableWidgetItem* eventType;
    QTableWidgetItem* windowHandle;
    QTableWidgetItem* hitRelease;
    QTableWidgetItem* eventCode;
    QTableWidgetItem* eventTime;
    QTableWidgetItem* windowTitle;
    QTableWidgetItem* XPos;
    QTableWidgetItem* YPos;


    for(int i=0;i<action_list.size();i++){
        QString action = action_list.at(i);
        QStringList actions = action.split(";");
        if(actions.isEmpty()) continue;
        if(QString::compare(actions.at(0),tr("K"))==0 && actions.size()==KEYBOARDACTIONFIELDCOUNT){
            eventType = new QTableWidgetItem("K");
            eventCode = new QTableWidgetItem(actions.at(1));
            hitRelease = new QTableWidgetItem(actions.at(2));
            eventTime = new QTableWidgetItem(actions.at(3));
            ui->tableWidget->setItem(i,0,eventType);
            ui->tableWidget->setItem(i,3,eventCode);
            ui->tableWidget->setItem(i,2,hitRelease);
            ui->tableWidget->setItem(i,4,eventTime);
        }else if(QString::compare(actions.at(0),tr("M"))==0 && actions.size()==MOUSEACTIONFIELDCOUNT){
            eventType = new QTableWidgetItem("M");
            windowHandle = new QTableWidgetItem(util::getHex(actions.at(1)));
            eventCode = new QTableWidgetItem(actions.at(2));
            eventTime = new QTableWidgetItem(actions.at(3));
            XPos = new QTableWidgetItem(actions.at(4));
            YPos = new QTableWidgetItem(actions.at(5));
            windowTitle = new QTableWidgetItem(actions.at(6));
            ui->tableWidget->setItem(i,0,eventType);
            ui->tableWidget->setItem(i,1,windowHandle);
            ui->tableWidget->setItem(i,3,eventCode);
            ui->tableWidget->setItem(i,4,eventTime);
            ui->tableWidget->setItem(i,6,XPos);
            ui->tableWidget->setItem(i,7,YPos);
            ui->tableWidget->setItem(i,5,windowTitle);
        }


    }


}

void AuTo::editScriptDetail(int row, int column){
    QString action = action_list.at(row);
    QStringList actions = action.split(";");

    if(QString::compare(actions.at(0),"K")==0){
        //如果不是keyboad记录的字段
        if(column==1||column==5||column==6||column==7) return;
        //将新的action覆盖原来的action//键盘事件累加顺序： 0324
        QString newAction = ui->tableWidget->item(row,0)->text()+tr(";")+ ui->tableWidget->item(row,3)->text()+tr(";")
                            + ui->tableWidget->item(row,2)->text()+tr(";")+ ui->tableWidget->item(row,4)->text();
        action_list[row] = newAction;
    }else if(QString::compare(actions.at(0),"M")==0){
         //如果不是mouse记录的字段
         if(column==2) return;
         //将新的action覆盖原来的action//鼠标事件累加顺序：   0134675
         QString newAction = ui->tableWidget->item(row,0)->text()+tr(";")+ ui->tableWidget->item(row,1)->text()+tr(";")
                             + ui->tableWidget->item(row,3)->text()+tr(";")+ ui->tableWidget->item(row,4)->text()+tr(";")
                              + ui->tableWidget->item(row,6)->text()+tr(";")+ ui->tableWidget->item(row,7)->text()+tr(";")
                                + ui->tableWidget->item(row,5)->text();
         action_list[row] = newAction;
    }


}

void AuTo::argsChange(){
    _replay_thread.setReplayTimes(setting_window->getRedoTimes());
    _replay_thread.setStart_delay(setting_window->getStartDelay());
}

void AuTo::replay()
{
    if(setting_window->getIfHideDuringRunning()) this->showMinimized();
    ui->statusBar->showMessage(tr("开始回放"),2000);
    _replay_thread.start();
    disableAllMenu();
    replayMenu->setEnabled(true);
    startReplayAC->setEnabled(false);
}


void AuTo::stop_replay(){
    if(_replay_thread.isRunning()){_replay_thread.terminate();_replay_thread.wait();
        enableAllMenu();
        startReplayAC->setEnabled(true);
        ui->statusBar->showMessage(tr("已中断回放"),2000);
    }
}

void AuTo::replayFinished(){
    enableAllMenu();
    startReplayAC->setEnabled(true);
    ui->statusBar->showMessage(tr("回放完毕"),2000);
}

void AuTo::exitApp(){
    if(_keyboard_recorded_thread.isRunning()){ _keyboard_recorded_thread.terminate(); _keyboard_recorded_thread.wait();}
    if(_mouse_recorded_thread.isRunning()){ _mouse_recorded_thread.terminate(); _mouse_recorded_thread.wait();}
    if(_replay_thread.isRunning()){ _replay_thread.terminate(); _replay_thread.wait();}
    qApp->quit();
}

void AuTo::disableAllMenu(){
    fileMenu->setEnabled(false);
    recordMenu->setEnabled(false);
    replayMenu->setEnabled(false);
    configMenu->setEnabled(false);
    viewMenu->setEnabled(false);
}

void AuTo::enableAllMenu(){
    fileMenu->setEnabled(true);
    recordMenu->setEnabled(true);
    replayMenu->setEnabled(true);
    configMenu->setEnabled(true);
    viewMenu->setEnabled(true);
}

/*设置窗口标题为：AUTO - 文件名的格式*/
void AuTo::setWindownTitleDashFileName(QString scriptFileName){
    this->setWindowTitle(WINDOWSOURCENAME+" - "+scriptFileName);
}


void AuTo::closeEvent(QCloseEvent *)
{
    this->exitApp();
}



int translateKeyCode(int key)   //按键转换值函数
{
    int k = key;
    bool legal = true;
    if(k >= Qt::Key_0 && k <= Qt::Key_9)
    {
    }
    else if(k >= Qt::Key_A && k <= Qt::Key_Z)
    {
    }
    else if(k >= Qt::Key_F1 && k <= Qt::Key_F24)
    {
        k &= 0x000000ff;
        k += 0x40;
    }
    else if(k == Qt::Key_Tab)
    {
        k = 0x09;
    }
    else if(k == Qt::Key_Backspace)
    {
        k = 0x08;
    }
    else if(k == Qt::Key_Return)
    {
        k = 0x0d;
    }
    else if(k <= Qt::Key_Down && k >= Qt::Key_Left)
    {
        int off = k - Qt::Key_Left;
        k = 0x25 + off;
    }
    else if(k == Qt::Key_Shift)
    {
        k = 0x10;
    }
    else if(k == Qt::Key_Control)
    {
        k = 0x11;
    }
    else if(k == Qt::Key_Alt)
    {
        k = 0x12;
    }
    else if(k == Qt::Key_Meta)
    {
        k = 0x5b;
    }
    else if(k == Qt::Key_Insert)
    {
        k = 0x2d;
    }
    else if(k == Qt::Key_Delete)
    {
        k = 0x2e;
    }
    else if(k == Qt::Key_Home)
    {
        k = 0x24;
    }
    else if(k == Qt::Key_End)
    {
        k = 0x23;
    }
    else if(k == Qt::Key_PageUp)
    {
        k = 0x21;
    }
    else if(k == Qt::Key_Down)
    {
        k = 0x22;
    }
    else if(k == Qt::Key_CapsLock)
    {
        k = 0x14;
    }
    else if(k == Qt::Key_NumLock)
    {
        k = 0x90;
    }
    else if(k == Qt::Key_Space)
    {
        k = 0x20;
    }
    else
        legal = false;

    if(!legal)
        return 0;
    return k;
}


