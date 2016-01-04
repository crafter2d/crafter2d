#ifndef TILESETSTREAMER_H
#define TILESETSTREAMER_H

class QTileSet;

class TileSetWriter
{
public:
    explicit TileSetWriter();

  // writing
    void write(const QTileSet &tileset);
};

#endif // TILESETSTREAMER_H
