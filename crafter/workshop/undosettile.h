#ifndef UNDOSETTILE_H
#define UNDOSETTILE_H

#include <QUndoCommand>
#include <QPoint>
#include <QVector>

#include "world/tilefield.h"

class TileWorld;

class UndoSetTile : public QUndoCommand
{
    struct TileInfo
    {
        int    tile;
        QPoint pos;
    };

    using Tiles = QVector<TileInfo>;

public:
    UndoSetTile(TileWorld &world, const QPoint& pos, TileField::Level level, int tile);

  // merging
    virtual int id() const override;
    virtual bool mergeWith(const QUndoCommand* pcommand) override;

  // commands
    virtual void undo() override;
    virtual void redo() override;

private:

  // data
    TileWorld&       mWorld;
    TileField::Level mLevel;
    Tiles            mTiles;
};

#endif // UNDOSETTILE_H
