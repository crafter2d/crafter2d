#include "undocleartile.h"

#include "world/tileworld.h"

UndoClearTile::UndoClearTile(TileWorld& world, const QPoint& pos, TileField::Level level):
    mWorld(world),
    mLevel(level),
    mTile(),
    mPos(pos)
{
}

// commands

void UndoClearTile::undo()
{
    mWorld.setTile(mPos, mLevel, mTile);
}

void UndoClearTile::redo()
{
    if ( !mTile.isValid() )
    {
        // only update the first time we get here, otherwise redo/undo/redo does not work correctly
        mTile = mWorld.getTile(mPos, mLevel);
    }
    mWorld.clearTile(mPos, mLevel);
}
