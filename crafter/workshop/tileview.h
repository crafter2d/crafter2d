#ifndef TILEVIEW_H
#define TILEVIEW_H

#include <QImage>
#include <QWidget>

#include <qpoint.h>
#include <qvector.h>

#include "world/tilefield.h"

#include "tile.h"

class TileBound;
class TileWorld;

namespace Ui {
class TileView;
}

class TileView : public QWidget
{
    Q_OBJECT

public:
    enum EditMode { eLayerMode, eBoundMode };
    enum BoundEdge { eNone, eLeft, eRight };

    TileView();
    virtual ~TileView();

  // get/set
    bool       hasWorld() const;
    TileWorld& getWorld();
    void       setWorld(TileWorld& maps);

    EditMode getEditMode() const;
    void     setEditMode(EditMode mode);

    QTileField::Level getLevel() const;
    void              setLevel(QTileField::Level level);

    void setActiveTile(const Tile& tile);

public slots:
    void worldUpdated();

protected:
  // overrides
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *pevent) override;
    virtual void mousePressEvent(QMouseEvent *pevent) override;
    virtual void mouseReleaseEvent(QMouseEvent*) override;
    virtual void mouseMoveEvent(QMouseEvent* pevent) override;

private:
  // operations
    void prepare();
    bool selectBoundEdge(QMouseEvent *pevent);
    void straightenBounds();

    Ui::TileView *ui;
    Tile          mTile;
    TileWorld*    mpWorld;
    EditMode      mEditMode;
    TileBound*    mpSelectedBound;
    BoundEdge     mSelectedEdge;
    QTileField::Level mLevel;
};

#endif // TILEVIEW_H
