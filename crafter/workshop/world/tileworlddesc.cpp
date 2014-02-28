#include "tileworlddesc.h"

#include <QDataStream>

QDataStream& operator<<(QDataStream& stream, const TileWorldDesc& desc)
{
    stream << desc.version << desc.name << desc.simulator << (int)desc.layerType;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, TileWorldDesc& desc)
{
    stream >> desc.version >> desc.name >> desc.simulator >> (int&)desc.layerType;
    return stream;
}
