#ifndef TILESETREADER_H
#define TILESETREADER_H

class QIODevice;

class TileSet;

class TileSetReader
{
public:
    TileSetReader(QIODevice &device);

  // reading
    TileSet* read();

private:

  // data
    QIODevice& mDevice;
};

#endif // TILESETREADER_H
