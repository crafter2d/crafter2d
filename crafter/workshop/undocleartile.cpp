#include "undocleartile.h"

#include "world/tileworld.h"

UndoClearTile::UndoClearTile(TileWorld& world, const QPoint& pos, TileField::Level level):
    mWorld(world),
    mLevel(level),
    mTiles()
{
    Tile tile = world.getTile(pos, mLevel);
    mTiles.append({tile, pos});
}

// merging

int UndoClearTile::id() const
{
    return 2;
}

bool UndoClearTile::mergeWith(const QUndoCommand* pcommand)
{
    const UndoClearTile* pclearcmd = dynamic_cast<const UndoClearTile*>(pcommand);
    if ( pclearcmd != nullptr && &pclearcmd->mWorld == &mWorld && pclearcmd->mLevel == mLevel )
    {
        mTiles.append(pclearcmd->mTiles);
        return true;
    }
    return false;
}

// commands

void UndoClearTile::undo()
{
    for ( auto& info : mTiles )
    {
        mWorld.setTile(info.pos, mLevel, info.tile);
    }
}

void UndoClearTile::redo()
{
    for ( auto& info : mTiles )
    {
        mWorld.clearTile(info.pos, mLevel);
    }
}
