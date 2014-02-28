#ifndef TILEWORLDWRITER_H
#define TILEWORLDWRITER_H

class QDataStream;
class QIODevice;

class TileMap;
class TileWorld;

class QTileWorldWriter
{
public:
    QTileWorldWriter(QIODevice &device);

  // writing
    void write(const TileWorld& world);

private:

  // writing
    void writeMap(QDataStream& stream, const TileMap& map);

  // data
    QIODevice& mDevice;
};

#endif // TILEWORLDWRITER_H
