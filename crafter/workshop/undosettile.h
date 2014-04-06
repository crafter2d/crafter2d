#ifndef UNDOSETTILE_H
#define UNDOSETTILE_H

#include <QUndoCommand>
#include <QPoint>

#include "world/tilefield.h"
#include "tile.h"

class TileWorld;

class UndoSetTile : public QUndoCommand
{
public:
    explicit UndoSetTile(TileWorld &world, const QPoint& pos, QTileField::Level level, const Tile &tile);

    virtual void undo() override;
    virtual void redo() override;

private:

  // data
    TileWorld&  mWorld;
    QPoint      mPos;
    Tile        mTile;
    QTileField::Level  mLevel;
};

#endif // UNDOSETTILE_H
