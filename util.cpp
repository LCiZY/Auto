#include "util.h"

util::util()
{

}


LPCWSTR util::char2wchar(const char *){

    return L" ";
}

QString util::getMinuteSecond(){

    QString result;
    int hour =QTime::currentTime().hour(); result = hour>9?(QString::number(hour)):(QString("0")+QString::number(hour));
    int minute = QTime::currentTime().minute(); result = minute>9?(result+QString(":")+QString::number(minute)):(result+QString(":")+QString("0")+QString::number(minute));
    int second = QTime::currentTime().second(); result = second>9?(result+QString(":")+QString::number(second)):(result+QString(":")+QString("0")+QString::number(second));
    return result;

}

QString util::getHex(int source){
    QString result;
    while (source!=0) {

        int temp = source%16;
        if(temp>9){
            result = QString((char)(97+temp-10)) + result;
        }else{
            result = QString::number(temp) + result;
        }
       source = source>>4;
    }
    return result;
}

QString util::getHex(QString source){
    bool ok;
    int temp = source.toInt(&ok,10);
    if(temp==0) return QString("0");
    return getHex(temp);
}
