#ifndef TILEVIEW_H
#define TILEVIEW_H

#include <QImage>
#include <QWidget>
#include <QUndoStack>
#include <QPoint>
#include <QVector>

#include "world/tilefield.h"

class MainWindow;
class TileBound;
class TileEntity;
class TileWorld;

namespace Ui {
class TileView;
}

class TileView : public QWidget
{
    Q_OBJECT

public:
    enum EditMode { eLayerMode, eBoundMode, eObjectMode, ePaintMode, eEraseMode };
    enum BoundEdge { eNone, eLeft, eRight };

    TileView();
    virtual ~TileView();

  // get/set
    bool       hasWorld() const;
    TileWorld& getWorld();
    void       setWorld(TileWorld& maps);

    EditMode getEditMode() const;
    void     setEditMode(EditMode mode);

    TileField::Level getLevel() const;
    void              setLevel(TileField::Level level);

    void setActiveTile(int tile);

  // operations
    void undo();
    void redo();

public slots:
    void worldUpdated();
    void setTransparency(double value);

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

  // query
    MainWindow* getMainWindow();

    Ui::TileView *ui;
    int           mTile;
    TileWorld*    mpWorld;
    EditMode      mEditMode;
    TileBound*    mpSelectedBound;
    TileEntity*   mpSelectedEntity;
    BoundEdge     mSelectedEdge;
    QUndoStack    mUndoStack;
    QPoint        mStartPos;
    TileField::Level mLevel;
    double           mTransparency;
};

#endif // TILEVIEW_H
