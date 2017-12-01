#include "errors.h"

Errors::Errors()
{

}

void Errors::Clear()
{
    errors.clear();
}

QString Errors::PopFirstInXML()
{
    if (isEmpty())
        return "";
    QString message = "<Err Id=\"" + errors.first().Id + "\" Message=\"" + errors.first().Message + "\"/>";
    errors.pop_front();
    return message;
}
QString Errors::PopAllInXML()
{
    QString message;
    while (!isEmpty())
    {
        message += PopFirstInXML();
    }
    errors.clear();
    return message;
}

void Errors::PushError(QString id, QString message)
{
    errors.append(ErrItem(id, message));
}

bool Errors::isEmpty()
{
    return errors.isEmpty();
}
