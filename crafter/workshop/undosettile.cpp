#include "undosettile.h"

#include "world/tileworld.h"

UndoSetTile::UndoSetTile(TileWorld& world, const QPoint &pos, TileField::Level level, const Tile &tile):
    QUndoCommand(NULL),
    mWorld(world),
    mPos(pos),
    mTile(tile),
    mLevel(level)
{
}

// - Operations

void UndoSetTile::undo()
{
    Tile prevTile = mWorld.getTile(mPos, mLevel);
    mWorld.setTile(mPos, mLevel, mTile);
    mTile = prevTile;
}

void UndoSetTile::redo()
{
    Tile prevTile = mWorld.getTile(mPos, mLevel);
    mWorld.setTile(mPos, mLevel, mTile);
    mTile = prevTile;
}
