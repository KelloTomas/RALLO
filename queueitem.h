#ifndef QUEUEITEM_H
#define QUEUEITEM_H
#include "queueitemenum.h"
#include <QString>
#include <QDateTime>
#include <QDebug>


class QueueItem
{
public:
    QString toString();
    QString toXML();
    QueueItem(QueueItemEnum action, QString value, QDateTime timeStamp = QDateTime::currentDateTimeUtc());
    QDateTime TimeStamp;
    QueueItemEnum Action;
    QString Value;
};

#endif // QUEUEITEM_H
