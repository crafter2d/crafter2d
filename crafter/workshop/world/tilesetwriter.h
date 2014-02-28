#ifndef TILESETSTREAMER_H
#define TILESETSTREAMER_H

class QIODevice;

class QTileSet;

class TileSetWriter
{
public:
    explicit TileSetWriter(QIODevice& device);

  // writing
    void write(const QTileSet &tileset);

private:

  //
    QIODevice& mDevice;
};

#endif // TILESETSTREAMER_H
