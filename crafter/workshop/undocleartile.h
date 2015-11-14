#ifndef UNDOCLEARTILE_H
#define UNDOCLEARTILE_H

#include <QPoint>
#include <QUndoCommand>

#include "world/tile.h"
#include "world/tilefield.h"

class TileWorld;

class UndoClearTile : public QUndoCommand
{
public:
    UndoClearTile(TileWorld &world, const QPoint &pos, TileField::Level level);

  // commands
    virtual void undo() override;
    virtual void redo() override;

private:

  // data
    TileWorld&          mWorld;
    TileField::Level    mLevel;
    Tile                mTile;
    QPoint              mPos;
};

#endif // UNDOCLEARTILE_H
