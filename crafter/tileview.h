#ifndef TILEVIEW_H
#define TILEVIEW_H

#include <QImage>
#include <QWidget>

#include <qpoint.h>
#include <qvector.h>

#include <engine/world/tileset.h>
#include <engine/world/layertype.h>

#include "tile.h"

class Bound;
class World;

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

    LayerLevel getLevel() const;
    void       setLevel(LayerLevel level);

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
    Bound*        mpSelectedBound;
    BoundEdge     mSelectedEdge;
    LayerLevel    mLevel;
};

#endif // TILEVIEW_H
