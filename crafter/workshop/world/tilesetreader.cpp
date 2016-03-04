#include "tilesetreader.h"

#include <QDir>
#include <QImage>
#include <QPixmap>
#include <QXmlStreamReader>

#include "../texturepacker/spriteatlas.h"
#include "../project.h"
#include "tileset.h"

TileSetReader::TileSetReader(QIODevice& device):
    mDevice(device)
{
}

// - Loading

TileSet* TileSetReader::read()
{
    QXmlStreamReader reader;
    reader.setDevice(&mDevice);

    TileSet* presult = new TileSet();

    const SpriteAtlas& atlas = Project::getActiveProject().getSpriteAtlas();

    while ( !reader.atEnd() )
    {
        QXmlStreamReader::TokenType type = reader.readNext();
        switch ( type )
        {
        case QXmlStreamReader::StartElement:
            {
                if ( reader.name() == "tileset" )
                {

                }
                else if ( reader.name() == "tile" )
                {
                    TileSet::Tile tile;
                    tile.name = reader.attributes().value("name").toString();
                    tile.spriteindex = atlas.lookup(tile.name);

                    if ( reader.attributes().hasAttribute("offsetx") )
                    {
                        tile.offset.setX(reader.attributes().value("offsetx").toInt());
                    }
                    if ( reader.attributes().hasAttribute("offsety") )
                    {
                        tile.offset.setY(reader.attributes().value("offsety").toInt());
                    }

                    presult->mTiles.append(qMove(tile));
                }
            }
            break;
        case QXmlStreamReader::Invalid:
            {
                // something is not correct
                QString error = reader.errorString();
                if ( !error.isEmpty() )
                {
                }
            }
            return NULL;
        }
    }

    presult->update();

    return presult;
}
