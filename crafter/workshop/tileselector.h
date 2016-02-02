#ifndef TILESELECTOR_H
#define TILESELECTOR_H

#include <QAbstractScrollArea>

class Tile;
class TileSet;

class TileSelector : public QAbstractScrollArea
{
    Q_OBJECT
public:
    explicit TileSelector(QWidget *parent = 0);

  // get/set
    const TileSet& getTileSet() const;
    void            setTileSet(const TileSet* ptileset);

signals:
    void tileSelected(int tile);

public slots:

protected:
  // overrides
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *e) override;
    virtual void scrollContentsBy(int dx, int dy) override;
    virtual void mousePressEvent(QMouseEvent *pevent) override;

private:

  // operations
    void tilesetChanged();

    void setSelection(int tile);
    void clearSelection();

  // data
    const TileSet* mpTileSet;
    int             mSelectedTile;
    int             mHorizontalTiles;
    int             mScrollPos;
};

#endif // TILESELECTOR_H
