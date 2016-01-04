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

void TileSetWriter::write(const QTileSet& tileset)
{
    QString path = Project::getActiveProject().getFilePath(tileset);
    QFile file(path);

    if ( file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QXmlStreamWriter writer(&file);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();

        writer.writeStartElement("tileset");
        writer.writeAttribute("width", QVariant(tileset.getTileSize().width()).toString());
        writer.writeAttribute("height", QVariant(tileset.getTileSize().height()).toString());
        writer.writeAttribute("count", QVariant(tileset.getTileCount()).toString());

        writer.writeStartElement("texture");
        writer.writeAttribute("name", tileset.getTileMap());
        writer.writeEndElement(); // </texture>

        writer.writeEndElement(); // </tileset>
        writer.writeEndDocument();
    }
}
