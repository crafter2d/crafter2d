#include "tilesetreader.h"

#include <QDir>
#include <QImage>
#include <QPixmap>
#include <QXmlStreamReader>

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

    while ( !reader.atEnd() )
    {
        QXmlStreamReader::TokenType type = reader.readNext();
        switch ( type )
        {
        case QXmlStreamReader::StartElement:
            {
                if ( reader.name() == "tileset" )
                {
                    QStringRef ref;
                    QSize size;
                    ref = reader.attributes().value("width");
                    if ( ref.isEmpty() )
                    {
                        // should not happen
                    }
                    else
                        size.setWidth(ref.toInt(NULL));

                    ref = reader.attributes().value("height");
                    if ( ref.isEmpty() )
                    {
                        // should not happen
                    }
                    else
                        size.setHeight(ref.toInt(NULL));

                    ref = reader.attributes().value("count");
                    if ( ref.isEmpty() )
                    {
                        // should not happen
                    }

                    presult->setTileSize(size);
                    presult->setTileCount(ref.toInt(NULL));
                }
                else if ( reader.name() == "texture" )
                {
                    QString mapname;
                    QStringRef ref = reader.attributes().value("name");
                    if ( !ref.isNull() && !ref.isEmpty() )
                    {
                        // import old format where texture name was an attribute
                        mapname = ref.toString();
                    }
                    else
                    {
                        mapname = reader.readElementText();
                    }

                    presult->setTileMap(mapname);
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

    return presult;
}
