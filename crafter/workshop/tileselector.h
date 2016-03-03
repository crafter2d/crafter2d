#ifndef TILESELECTOR_H
#define TILESELECTOR_H

#include <QAbstractScrollArea>

class Tile;
class TileSet;
class QAbstractScrollArea;

class TileSelector : public QWidget
{
    Q_OBJECT
public:
    explicit TileSelector(QAbstractScrollArea* parea);

  // get/set
    const TileSet& getTileSet() const;
    void           setTileSet(const TileSet* ptileset);

signals:
    void tileSelected(int tile);

public slots:

protected:
  // overrides
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *e) override;
    virtual void mousePressEvent(QMouseEvent *pevent) override;

private:

  // operations
    void tilesetChanged();

    void setSelection(int tile);
    void clearSelection();

  // data
    QAbstractScrollArea* mpScrollArea;
    const TileSet*       mpTileSet;
    int                  mSelectedTile;
    int                  mHorizontalTiles;
};

#endif // TILESELECTOR_H
