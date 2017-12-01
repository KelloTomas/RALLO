#include "queueactions.h"


QueueActions::QueueActions()
{
}

void QueueActions::append(QueueItem item)
{
    queue.append(item);
}
void QueueActions::clear()
{
    queue.clear();
}
bool QueueActions::empty()
{
    return queue.empty();
}

QString QueueActions::getAllInXML()
{
    QString msg;
    foreach(QueueItem item, queue)
    {
        /*
        if (DebugFunctionGetQueue)
            qDebug() << "GetQue - item: " << item.toString();
            */
        msg.append(item.toXML());
    }
    queue.clear();
    return msg;
}

