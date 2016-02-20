#ifndef SPRITEATLASREADER_H
#define SPRITEATLASREADER_H

#include <QString>

class SpriteAtlas;
class SpriteSheet;

class SpriteAtlasReader
{
public:

    static void read(const QString& path, SpriteAtlas& atlas);

private:
    SpriteAtlasReader(const QString &path);

    void readAtlas(SpriteAtlas &atlas);

  // data
    QString mPath;
};

#endif // SPRITEATLASREADER_H
