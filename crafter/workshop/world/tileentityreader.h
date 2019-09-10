#ifndef TILEENTITYREADER_H
#define TILEENTITYREADER_H

class QFile;
class QXmlStreamReader;

class TileWorld;

class TileEntityReader
{
public:
    TileEntityReader();

    void load(QFile& input, TileWorld& world);

private:

    void loadEntity(QXmlStreamReader& stream, TileWorld& world);
};

#endif // TILEENTITYREADER_H
