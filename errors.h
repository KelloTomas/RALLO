#ifndef ERROR_H
#define ERROR_H

#include <QList>
#include <QString>
#include "erritem.h"

class Errors
{
private:
    QList<ErrItem> errors;
public:
    Errors();
    void Clear();
    bool isEmpty();
    QString PopFirstInXML();
    QString PopAllInXML();
    void PushError(QString Id, QString Message);
};

#endif // ERROR_H
