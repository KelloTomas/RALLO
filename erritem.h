#ifndef ERRITEM_H
#define ERRITEM_H

#include <QString>

class ErrItem
{
public:
    ErrItem(QString id, QString message)
    {
        Id = id;
        Message = message;
    }

    QString Id;
    QString Message;
};

#endif // ERRITEM_H
