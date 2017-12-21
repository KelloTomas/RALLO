#include "erritem.h"

QString ErrItem::getXML()
{
    return "<Err Id=\"" + Id + "\" Message=\"" + Message + "\"/>";
}
