#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include<QTimer>
#include<QLabel>
#include<QMovie>
#include<QFrame>
#include<QGraphicsDropShadowEffect>
#include<QIntValidator>

namespace Ui {
class setting;
}

class setting : public QWidget
{
    Q_OBJECT

public:
    explicit setting(QWidget *parent = 0);
    ~setting();

    bool getIfHideDuringRunning();
    int getRedoTimes();
    int getStartDelay();


public slots:
    void ifHideDuringRunningStateChange(int);
    void redo_times_change(QString);
    void start_delay_change(QString);

private:
         Ui::setting *ui;
         QGraphicsDropShadowEffect* effect;
         QIntValidator *int_valid;

         bool ifHideDuringRunning;
         int redo_times;
         int start_delay;


signals:
     void argsChange();

};

#endif // SETTING_H
