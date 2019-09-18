#include "tilemapdesc.h"

#include <QDataStream>

TileMapDesc::TileMapDesc():
    name(),
    effect(),
    tileset(),
    size(),
    flags(0)
{
}

QDataStream& operator<<(QDataStream& stream, const TileMapDesc& desc)
{
    stream << desc.name << desc.effect << desc.tileset << desc.size << desc.flags;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, TileMapDesc& desc)
{
    stream >> desc.name >> desc.effect >> desc.tileset >> desc.size >> desc.flags;
    return stream;
}
