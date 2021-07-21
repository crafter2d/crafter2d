#include "spritesheetreader.h"

#include <stdexcept>

#include <QDir>
#include <QFile>
#include <QXmlStreamReader>

#include "../project.h"

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
                        int index = imageName.indexOf(QDir::separator());
                        if ( index != -1 )
                        {
                            imageName = imageName.right(imageName.length() - index - 1);
                        }
                        bool loaded = sheet.load(Project::getActiveProject().getBasePath() + QDir::separator() + imageName + ".png");
                        if (!loaded) {
                            throw new std::runtime_error("Could not load tile atlas");
                        }
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
