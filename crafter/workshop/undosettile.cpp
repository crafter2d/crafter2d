#include "undosettile.h"

#include "world/tileworld.h"

UndoSetTile::UndoSetTile(TileWorld& world, const QPoint &pos, TileField::Level level, int tile):
    QUndoCommand(NULL),
    mWorld(world),
    mLevel(level),
    mTiles()
{
    mTiles.append({tile, pos});
}

// merging

int UndoSetTile::id() const
{
    return 1;
}

bool UndoSetTile::mergeWith(const QUndoCommand* pcommand)
{
    const UndoSetTile* psetcmd = dynamic_cast<const UndoSetTile*>(pcommand);
    if ( psetcmd != nullptr && &psetcmd->mWorld == &mWorld && psetcmd->mLevel == mLevel )
    {
        mTiles.append(psetcmd->mTiles);
        return true;
    }
    return false;
}

// - Operations

void UndoSetTile::undo()
{
    redo();
}

void UndoSetTile::redo()
{
    for ( auto& info : mTiles )
    {
        int prevTile = mWorld.getTile(info.pos, mLevel);
        mWorld.setTile(info.pos, mLevel, info.tile);
        info.tile = prevTile;
    }
}
