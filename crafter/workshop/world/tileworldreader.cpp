#include "tileworldreader.h"

#include <QIODevice>

#include "../project.h"

#include "tilemap.h"
#include "tileworld.h"
#include "tilebound.h"
#include "tileworlddesc.h"
#include "tilemapdesc.h"
#include "tilefield.h"

QTileWorldReader::QTileWorldReader(QIODevice& device):
    mDevice(device)
{
}

// - Get/set

const QString& QTileWorldReader::baseDir() const
{
    return mBaseDir;
}

void QTileWorldReader::setBaseDir(const QString& basedir)
{
    mBaseDir = basedir;
}

// - Reading

TileWorld* QTileWorldReader::read()
{
    Q_ASSERT(mDevice.isOpen());

    QDataStream stream(&mDevice);
    stream.setByteOrder(QDataStream::LittleEndian);

    TileWorldDesc worlddesc;
    stream >> worlddesc;

    // ensure we are reading the correct version
    if ( worlddesc.version != 1 )
        return nullptr;

    TileWorld* presult = new TileWorld(worlddesc);

    int layerCount;
    stream >> layerCount;
    for ( int index = 0; index < layerCount; ++index )
    {
        TileMapDesc desc;
        stream >> desc;

        TileMap* pmap = new TileMap(desc);
        if ( desc.flags == TileMapDesc::eTileSet )
        {
            stream >> pmap->getField();
        }

        presult->addMap(pmap);
    }

    // read in the bounds

    int bounds;
    stream >> bounds;
    for ( int index = 0; index < bounds; ++index )
    {
        QPointF left, right;
        stream >> left >> right;
        TileBound* pbound = new TileBound(left, right);
        presult->addBound(pbound);
    }

    return presult;
}
