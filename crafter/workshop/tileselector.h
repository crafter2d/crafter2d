#ifndef TILESELECTOR_H
#define TILESELECTOR_H

#include <QWidget>

class QTileSet;

class TileSelector : public QWidget
{
    Q_OBJECT
public:
    explicit TileSelector(QWidget *parent = 0);

  // get/set
    const QTileSet& getTileSet() const;
    void            setTileSet(const QTileSet* ptileset);

  // overrides
    virtual QSize sizeHint() const override;

signals:

public slots:

protected:
  // overrides
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *e) override;

private:

  // operations
    void tilesetChanged();

  // data
    const QTileSet* mpTileSet;
    int             mHorizontalTiles;
};

#endif // TILESELECTOR_H
