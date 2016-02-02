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

        writer.writeStartElement("texture");
        writer.writeAttribute("name", tileset.getImagePath());
        writer.writeEndElement(); // </texture>

        writer.writeEndElement(); // </tileset>
        writer.writeEndDocument();
    }
}
