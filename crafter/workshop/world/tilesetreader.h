#ifndef TILESETREADER_H
#define TILESETREADER_H

class QIODevice;

class QTileSet;

class TileSetReader
{
public:
    TileSetReader(QIODevice &device);

  // reading
    QTileSet* read();

private:

  // data
    QIODevice& mDevice;
};

#endif // TILESETREADER_H
