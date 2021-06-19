#ifndef AUTO_H
#define AUTO_H

#include<QMainWindow>

#include<QSoundEffect>
#include<QLabel>
#include<QTableWidgetItem>
#include<QActionGroup>

#include<keyboard_recorded_thread.h>
#include<mouse_recorded_thread.h>
#include<replay_thread.h>
#include<setting.h>


#include<QPixmap>
#include <QCloseEvent>
#include<QFileDialog>
#include<QFile>
#include<QIODevice>

namespace Ui {
class AuTo;
}

class AuTo : public QMainWindow
{
    Q_OBJECT

public:

    explicit AuTo(QWidget *parent = 0);
    ~AuTo();
    friend LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);



    void Sleep(int msec);
signals:


private slots:
void recordAction();
void recordActionFinished();
void stopRecordAction();
void showSettingWindow();
void open_script_file();
void script_file_save();
void script_file_save_as();
void replay();
void stop_replay();
void replayFinished();
void argsChange();
void showScriptDetail(bool);
void shutScriptDetail(bool);
void createScriptDetailList();
void editScriptDetail(int row,int column);
void exitApp();

private:
bool read_actions_from_script_file();
bool write_actions_to_script_file(QString script_file_path);
void disableAllMenu();
void enableAllMenu();
void setWindownTitleDashFileName(QString scriptFileName);

protected:
void closeEvent(QCloseEvent *event);

private:
    Ui::AuTo *ui;

    /*动作完成音效*/
    QSoundEffect endingmusic;
    /*遮罩层*/
    QWidget*   mpShadeWindow;

    /*子窗口*/
    setting *setting_window;

    /*“文件”主菜单*/
    QMenu *fileMenu;
    QAction *openScriptFileAC;
    QAction *saveScriptFileAC;
    QAction *saveAsScriptFileAC;
    QAction *exitAC;

     /*“录制”主菜单*/
    QMenu *recordMenu;
    QAction *startRecAC;
    QAction *stopRecAC;

    /*“回放”主菜单*/
    QMenu *replayMenu;
    QAction *startReplayAC;
    QAction *stopReplayAC;

    /*“选项”主菜单*/
    QMenu *configMenu;
    QAction *settingAC;

    /*“视图”主菜单*/
    QMenu *viewMenu;
    /*“脚本视图”子菜单*/
    QMenu *scriptViewMenu;
    QActionGroup *scriptViewACGroup;
    QAction *showScriptViewAC;
    QAction *shutScriptViewAC;


    /*状态栏*/
    QLabel *permanent;

    /*键盘录制线程、鼠标录制线程、回放线程*/
    keyboard_recorded_thread _keyboard_recorded_thread;
    mouse_recorded_thread _mouse_recorded_thread;
    replay_thread _replay_thread;

    /*控制位：如果鼠标和键盘录制线程都结束，则recordFinishCount%2==0 */
    int recordFinishCount;

    /*鼠标和键盘事件字段的数量*/
    const int MOUSEACTIONFIELDCOUNT = 8;
    const int KEYBOARDACTIONFIELDCOUNT = 4;

    /*脚本详情表中的列数*/
    const int SCRIPTLISTCOLUMNSNUM = 8;

    /*原名*/
    const QString WINDOWSOURCENAME = "AUTO";

    /*脚本文件的路径*/
    QString scriptFilePath;



};







#endif // AuTo_H
