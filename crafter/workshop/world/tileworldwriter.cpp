#include "tileworldwriter.h"

#include <QDataStream>

#include "tilebound.h"
#include "tilemap.h"
#include "tileworld.h"
#include "tileworlddesc.h"

QTileWorldWriter::QTileWorldWriter(QIODevice& device):
    mDevice(device)
{
}

// - Writing

void QTileWorldWriter::write(const TileWorld& world)
{
    Q_ASSERT(mDevice.isOpen() && mDevice.isWritable());
    QDataStream stream(&mDevice);
    stream.setByteOrder(QDataStream::LittleEndian);

    const TileWorldDesc& worlddesc = world.getDesc();
    stream << worlddesc;

    int mapcount = world.getMapCount();
    stream << mapcount;
    for ( int index = 0; index < mapcount; ++index )
    {
        const TileMap& tilemap = world.getMap(index);
        writeMap(stream, tilemap);
    }

    int boundcount = world.getBoundCount();
    stream << boundcount;
    for ( int index = 0; index < boundcount; ++index )
    {
        const TileBound& bound = world.getBound(index);
        stream << bound.left() << bound.right();
    }
}

void QTileWorldWriter::writeMap(QDataStream& stream, const TileMap& map)
{
    const TileMapDesc& desc = map.getDesc();
    const TileField& field = map.getField();
    stream << desc << field;
}
