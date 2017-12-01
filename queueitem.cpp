#include "queueitem.h"

QueueItem::QueueItem(QueueItemEnum action, QString value, QDateTime timeStamp)
{
    TimeStamp = timeStamp;
    Action = action;
    Value = value;
}

QString QueueItem::toXML()
{
    switch (Action)
    {
    case QueueItemEnum::JustStarted:
        return "<Que TimeStamp=\"" + TimeStamp.toString("yyyy-MM-ddThh:mm:ss") + "\"><JustStarted/></Que>";
    case QueueItemEnum::ButtonClick:
        return "<Que TimeStamp=\"" + TimeStamp.toString("yyyy-MM-ddThh:mm:ss") + "\"><ButtonClick Id=\"" + Value + "\" /></Que>";
    case QueueItemEnum::CardRead:
        return "<Que TimeStamp=\"" + TimeStamp.toString("yyyy-MM-ddThh:mm:ss") + "\"><CardRead Number=\"" + Value + "\" /></Que>";
    default:
        qDebug() << "Unknow QueueItemEnum with value: " << Value;
        return "<Que TimeStamp=\"" + TimeStamp.toString("yyyy-MM-ddThh:mm:ss") + "\"><" + "Action" + " Value=\"" + Value + "\" /></Que>";
    }
}

QString QueueItem::toString()
{
    return "At " + TimeStamp.toString() + " was action with value: " + Value;
}
