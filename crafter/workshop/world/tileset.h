#ifndef TILESET_H
#define TILESET_H

#include "../resource.h"

#include <QImage>
#include <QSize>

class QTileSet : public Resource
{
    Q_OBJECT

public:
    QTileSet();

  // get/set
    bool          hasTexture() const;
    const QImage& getTexture() const;
    void          setTexture(const QImage& texture);

    const QString& getTileMap() const;
    void           setTileMap(const QString& map, bool forceReload = false);

    const QSize& getTileSize() const;
    void         setTileSize(const QSize& size);

    int  getTileCount() const;
    void setTileCount(int count);

private:

  // operations
    void loadTexture();

  // data
    QString mTileMap;
    QImage  mTexture;
    QSize   mTileSize;
    int     mTileCount;
};

#endif // TILESET_H
