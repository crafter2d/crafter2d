#include "tilemapmodel.h"

#include "tilemap.h"
#include "tileworld.h"

TileMapModel::TileMapModel():
    QAbstractTableModel(NULL),
    mpWorld(NULL)
{
}

// - Get/set

void TileMapModel::setWorld(TileWorld* pworld)
{
    if ( mpWorld != pworld )
    {
        beginResetModel();
        mpWorld = pworld;
        endResetModel();
    }
}

bool TileMapModel::hasWorld() const
{
    return mpWorld != NULL;
}

TileWorld* TileMapModel::getWorld()
{
    return mpWorld;
}

void TileMapModel::clearWorld()
{
    beginResetModel();
    mpWorld = NULL;
    endResetModel();
}

// - Operations

void TileMapModel::moveUp(const QModelIndex& index)
{
    if ( index.row() > 0 )
    {
        beginMoveRows(index.parent(), index.row(), index.row(), index.parent(), index.row() - 1);
        mpWorld->moveUp(index.row());
        endMoveRows();
    }
}

void TileMapModel::moveDown(const QModelIndex& index)
{
    if ( index.row() < rowCount(index.parent()) - 1 )
    {
        beginMoveRows(index.parent(), index.row(), index.row(), index.parent(), index.row() + 2);
        mpWorld->moveDown(index.row());
        endMoveRows();
    }
}

// - Overrides

bool TileMapModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if ( mpWorld != NULL )
    {
        beginRemoveRows(parent, row, count);
        TileMap& map = mpWorld->getMap(row);
        mpWorld->removeMap(map);
        delete &map;
        endRemoveRows();
        return true;
    }

    return false;
}

int TileMapModel::rowCount(const QModelIndex &/*parent*/) const
{
    if ( mpWorld != NULL )
    {
        return mpWorld->getMapCount();
    }
    return 0;
}

int TileMapModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant TileMapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    switch (section)
    {
        case 0: return tr("Visible");
        case 1: return tr("Name");
    }

    return QVariant();
}

QVariant TileMapModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
            return QVariant();

    switch ( index.column() )
    {
    case 0:
        return QVariant(true);

    case 1:
        TileMap& map = mpWorld->getMap(index.row());
        QString name = map.getName();
        if ( name.isEmpty() )
        {
            name = tr("<no name>");
        }
        return QVariant(name);
    }

    return QVariant();
}
