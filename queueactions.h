#ifndef QUEUEACTIONS_H
#define QUEUEACTIONS_H

#include<QQueue>
#include"queueitem.h"

class QueueActions
{
private:
    QQueue<QueueItem> queue;
public:
    QueueActions();
    void append(QueueItem item);
    void clear();
    bool empty();
    QString getAllInXML();
};

#endif // QUEUEACTIONS_H
