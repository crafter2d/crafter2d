#ifndef SPRITESHEETWRITER_H
#define SPRITESHEETWRITER_H

#include <QString>

class SpriteAtlas;
class SpriteSheet;
class QXmlStreamWriter;

class SpriteAtlasWriter
{
public:
    static const QString& atlasFilename();

    static void write(const QString& path, const SpriteAtlas& atlas);

private:
    SpriteAtlasWriter(const QString& path);

    void writeAtlas(const SpriteAtlas &atlas);
    void writeSheet(QXmlStreamWriter& writer, const SpriteSheet& sheet);

  // data
    QString mPath;
    int mSheetIndex;
};

#endif // SPRITESHEETWRITER_H
