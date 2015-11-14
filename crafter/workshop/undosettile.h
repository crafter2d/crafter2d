#ifndef UNDOSETTILE_H
#define UNDOSETTILE_H

#include <QUndoCommand>
#include <QPoint>

#include "world/tile.h"
#include "world/tilefield.h"

class TileWorld;

class UndoSetTile : public QUndoCommand
{
public:
    explicit UndoSetTile(TileWorld &world, const QPoint& pos, TileField::Level level, const Tile &tile);

  // commands
    virtual void undo() override;
    virtual void redo() override;

private:

  // data
    TileWorld&  mWorld;
    QPoint      mPos;
    Tile        mTile;
    TileField::Level  mLevel;
};

#endif // UNDOSETTILE_H
