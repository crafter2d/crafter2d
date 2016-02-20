#include "spritesheetreader.h"

#include <QDir>
#include <QFile>
#include <QXmlStreamReader>

#include "sprite.h"
#include "spriteatlas.h"
#include "spritesheet.h"
#include "spritesheetwriter.h"

void SpriteAtlasReader::read(const QString& path, SpriteAtlas& atlas)
{
    SpriteAtlasReader reader(path);
    reader.readAtlas(atlas);
}

SpriteAtlasReader::SpriteAtlasReader(const QString& path):
    mPath(path)
{
}

void SpriteAtlasReader::readAtlas(SpriteAtlas& atlas)
{
    QFile file(mPath + QDir::separator() + SpriteAtlasWriter::atlasFilename());

    if ( file.open(QIODevice::ReadOnly) )
    {
        QXmlStreamReader reader(&file);
        SpriteSheet sheet;

        while ( !reader.atEnd() )
        {
            QXmlStreamReader::TokenType type = reader.readNext();
            switch ( type )
            {
            case QXmlStreamReader::StartElement:
                {
                    if ( reader.name() == "tilesheet" )
                    {
                        sheet.clear();

                        QString imageName = reader.attributes().value("name").toString();
                        sheet.load(mPath + QDir::separator() + imageName);
                    }
                    else if ( reader.name() == "tile" )
                    {
                        Sprite sprite;
                        sprite.mName = reader.attributes().value("name").toString();

                        int x = reader.attributes().value("x").toInt();
                        int y = reader.attributes().value("y").toInt();
                        int w = reader.attributes().value("w").toInt();
                        int h = reader.attributes().value("h").toInt();
                        sprite.mCoordinates.setRect(x, y, w, h);

                        if ( reader.attributes().hasAttribute("r") )
                        {
                            if ( reader.attributes().value("r") == "true" )
                            {
                                sprite.mRotated = true;
                            }
                        }

                        sheet.addSprite(sprite);
                    }
                }
                break;
            case QXmlStreamReader::EndElement:
                {
                    if ( reader.name() == "tilesheet" )
                    {
                        atlas.append(qMove(sheet));
                    }
                }
                break;
            }
        }
    }
}
