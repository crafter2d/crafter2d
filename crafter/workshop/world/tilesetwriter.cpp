#include "tilesetwriter.h"

#include <QFile>
#include <QIODevice>
#include <QXmlStreamWriter>
#include <QVariant>

#include "../project/projectmanager.h"
#include "../project.h"

#include "tileset.h"

TileSetWriter::TileSetWriter()
{
}

// - Storage

void TileSetWriter::write(const TileSet& tileset)
{
    QString path = Project::getActiveProject().getFilePath(tileset);
    QFile file(path);

    if ( file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QXmlStreamWriter writer(&file);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();

        writer.writeStartElement("tileset");

        const QSize& size = tileset.getTileSize();
        writer.writeAttribute("tilewidth", QString::number(size.width()));
        writer.writeAttribute("tileheight", QString::number(size.height()));

        for ( int index = 0; index < tileset.mTiles.size(); ++index )
        {
            auto& tile = tileset.mTiles[index];

            writer.writeStartElement("tile");
            writer.writeAttribute("id", QString::number(index));
            writer.writeAttribute("name", tile.name);
            writer.writeEndElement(); // </tile>
        }

        writer.writeEndElement(); // </tileset>
        writer.writeEndDocument();
    }
}
