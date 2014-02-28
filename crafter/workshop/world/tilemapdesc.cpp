#include "tilemapdesc.h"

#include <QDataStream>

QDataStream& operator<<(QDataStream& stream, const TileMapDesc& desc)
{
    stream << desc.name << desc.effect << desc.tileset << desc.size;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, TileMapDesc& desc)
{
    stream >> desc.name >> desc.effect >> desc.tileset >> desc.size;
    return stream;
}
