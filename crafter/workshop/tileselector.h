#ifndef TILESELECTOR_H
#define TILESELECTOR_H

#include <QAbstractScrollArea>

class Tile;
class TileSet;
class QAbstractScrollArea;

class TileContextInfo
{
public:
    TileContextInfo();

    QPoint pos;
    TileSet* tileset;
    int tile;
};

class TileSelector : public QWidget
{
    Q_OBJECT
public:
    explicit TileSelector(QAbstractScrollArea* parea);
    virtual ~TileSelector();

  // get/set
    const TileSet& getTileSet() const;
    void           setTileSet(TileSet *ptileset);

signals:
    void tileSelected(int tile);
    void contextMenuPopup(TileContextInfo& info);

protected:
  // overrides
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *e) override;
    virtual void contextMenuEvent(QContextMenuEvent *pevent) override;
    virtual void mousePressEvent(QMouseEvent *pevent) override;

private slots:
    void on_tileset_updated();

private:

  // operations
    void tilesetChanged();

    void setSelection(int tile);
    void clearSelection();

  // data
    QAbstractScrollArea* mpScrollArea;
    TileSet*             mpTileSet;
    int                  mSelectedTile;
    int                  mHorizontalTiles;
};

#endif // TILESELECTOR_H
