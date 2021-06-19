#ifndef QDOUBLEVALID_H
#define QDOUBLEVALID_H

#include <QObject>
#include<QDoubleValidator>

class qdoublevalid : public QDoubleValidator
{
    Q_OBJECT
public:
    qdoublevalid(QObject *parent);
    ~qdoublevalid();
    virtual QValidator::State validate(QString &input, int &pos) const;
};

#endif // QDOUBLEVALID_H
