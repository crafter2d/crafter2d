#ifndef UNDOSETTILE_H
#define UNDOSETTILE_H

#include <QUndoCommand>
#include <QPoint>

#include "tile.h"

class UndoSetTile : public QUndoCommand
{
public:
    explicit UndoSetTile(TileWorld &world, const QPoint& pos, LayerLevel level, const Tile &tile);

    virtual void undo() override;
    virtual void redo() override;

private:

  // data
    TileWorld&  mWorld;
    QPoint      mPos;
    Tile        mTile;
    LayerLevel  mLevel;
};

#endif // UNDOSETTILE_H
