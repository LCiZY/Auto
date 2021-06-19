#include "setting.h"
#include "ui_setting.h"

setting::setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::setting)
{
    ui->setupUi(this);
    effect = new QGraphicsDropShadowEffect; effect->setBlurRadius(6); effect->setColor(Qt::black);
    effect->setOffset(0,0); //为控件设置阴影效果 this 可以换成Qwidget 的任意派生类

    int_valid=new QIntValidator; int_valid->setRange(1,65536);
    ui->lineEdit_redo_times->setValidator(int_valid);
    ui->lineEdit_start_delay->setValidator(int_valid);

    this->setWindowTitle(tr("Setting"));


    redo_times = 1;
    start_delay = 2;
    ifHideDuringRunning = false;

    connect(ui->checkBox_ifHideDuringRunning,SIGNAL(stateChanged(int)),this,SLOT(ifHideDuringRunningStateChange(int)));
    connect(ui->lineEdit_redo_times,SIGNAL(textEdited(QString)),this,SLOT(redo_times_change(QString)));
    connect(ui->lineEdit_start_delay,SIGNAL(textEdited(QString)),this,SLOT(start_delay_change(QString)));


}

setting::~setting()
{
    delete ui;
}

void setting::ifHideDuringRunningStateChange(int){
    ifHideDuringRunning = ui->checkBox_ifHideDuringRunning->isChecked();
     emit argsChange();
}

void setting::redo_times_change(QString){
    redo_times = ui->lineEdit_redo_times->text().toInt();
     emit argsChange();
}

void setting::start_delay_change(QString){
    start_delay = ui->lineEdit_start_delay->text().toInt();
    emit argsChange();
}

bool setting::getIfHideDuringRunning(){
    return this->ifHideDuringRunning;
}

int setting::getRedoTimes(){
    return this->redo_times;
}

int setting::getStartDelay(){
    return this->start_delay;
}
