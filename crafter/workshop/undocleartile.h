#ifndef UNDOCLEARTILE_H
#define UNDOCLEARTILE_H

#include <QPoint>
#include <QUndoCommand>
#include <QVector>

#include "world/tile.h"
#include "world/tilefield.h"

class TileWorld;

class UndoClearTile : public QUndoCommand
{
    struct TileInfo
    {
        Tile   tile;
        QPoint pos;
    };

    using Tiles = QVector<TileInfo>;

public:
    UndoClearTile(TileWorld &world, const QPoint &pos, TileField::Level level);

  // merging
    virtual int id() const override;
    virtual bool mergeWith(const QUndoCommand* pcommand) override;

  // commands
    virtual void undo() override;
    virtual void redo() override;

private:

  // data
    TileWorld&          mWorld;
    TileField::Level    mLevel;
    Tiles               mTiles;
};

#endif // UNDOCLEARTILE_H
