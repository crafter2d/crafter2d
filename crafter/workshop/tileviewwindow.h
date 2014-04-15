#ifndef TILEVIEWWINDOW_H
#define TILEVIEWWINDOW_H

#include <QScrollArea>

class Tile;
class TileView;
class TileWorld;

class TileViewWindow : public QScrollArea
{
    Q_OBJECT
public:
    explicit TileViewWindow(QWidget *parent = 0);

  // get/set
    bool       hasWorld() const;
    TileWorld& getWorld();
    void       setWorld(TileWorld& world);

    TileView& getTileView();

    void setActiveTile(const Tile *tile);

signals:

public slots:

private:

  // data
    TileView*  mpTileView;
};

#endif // TILEVIEWWINDOW_H
