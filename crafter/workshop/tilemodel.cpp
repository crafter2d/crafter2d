#include "tilemodel.h"

#include "tile.h"
#include "tilemap.h"

TileModel::TileModel(QObject *parent):
    QAbstractListModel(parent),
    mpMap(NULL)
{
}

// - Get/set

void TileModel::setMap(TileMap* pmap)
{
    if ( mpMap != pmap )
    {
        beginResetModel();
        mpMap = pmap;
        endResetModel();
    }
}

void TileModel::clearMap()
{
    setMap(NULL);
}

// - Implementation

int TileModel::rowCount(const QModelIndex &parent) const
{
    if ( mpMap != NULL )
    {
        return mpMap->getTileCount();
    }
    return 0;
}

QVariant TileModel::data(const QModelIndex &index, int role) const
{
    if ( role != Qt::DisplayRole )
        return QVariant();

    if ( mpMap != NULL )
    {
        const Tile& tile = mpMap->getTile(index.row());

        QVariant result;
        result.setValue(tile);
        return result;
    }

    return QVariant();
}
