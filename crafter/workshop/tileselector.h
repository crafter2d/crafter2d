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
    void tileSelected(const Tile* ptile);

public slots:

protected:
  // overrides
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *e) override;
    virtual void scrollContentsBy(int dx, int dy) override;
    virtual void mousePressEvent(QMouseEvent *pevent) override;

private:

  // get/set
    const Tile& get(int index);

  // operations
    void tilesetChanged();

    void setSelection(int index);
    void clearSelection();

  // data
    const TileSet* mpTileSet;
    int             mSelectedIndex;
    int             mHorizontalTiles;
    int             mScrollPos;
};

#endif // TILESELECTOR_H
