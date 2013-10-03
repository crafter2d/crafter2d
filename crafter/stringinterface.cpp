
#include "stringinterface.h"

String toString(const QString& str)
{
    return String::fromUtf8(str.toUtf8().data());
}

QString toQString(const String& str)
{
    return QString(str.toUtf8().c_str());
}
