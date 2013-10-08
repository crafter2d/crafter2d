#ifndef STRINGINTERFACE_H
#define STRINGINTERFACE_H

#include <QString>
#include <core/string/string.h>

String toString(const QString& str);
QString toQString(const String& str);

#endif // STRINGINTERFACE_H
