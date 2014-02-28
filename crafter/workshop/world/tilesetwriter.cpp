#include "tilesetwriter.h"

#include <QIODevice>
#include <QXmlStreamWriter>
#include <QVariant>

#include "tileset.h"

TileSetWriter::TileSetWriter(QIODevice& device):
    mDevice(device)
{
}

// - Storage

void TileSetWriter::write(const QTileSet& tileset)
{
    QXmlStreamWriter writer(&mDevice);
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
