#ifndef TILESETSTREAMER_H
#define TILESETSTREAMER_H

class TileSet;

class TileSetWriter
{
public:
    explicit TileSetWriter();

  // writing
    void write(const TileSet &tileset);
};

#endif // TILESETSTREAMER_H
