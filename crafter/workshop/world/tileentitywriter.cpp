#include "tileentitywriter.h"

#include <QFile>
#include <QXmlStreamWriter>

#include "../entity/entity.h"

#include "tileentity.h"
#include "tilemap.h"

TileEntityWriter::TileEntityWriter()
{

}

void TileEntityWriter::write(QFile& file, TileWorld::Entities& entities)
{
    file.open(QIODevice::WriteOnly);

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("worldinfo");
    stream.writeStartElement("entities");

    for ( auto pentity : entities )
    {
        QPoint pos = pentity->getCenterPos();

        stream.writeStartElement("entity");
        stream.writeAttribute("name", pentity->getEntity().getResourceName());
        stream.writeAttribute("x", QString::number(pos.x()));
        stream.writeAttribute("y", QString::number(pos.y()));
        stream.writeAttribute("scale", QString::number(static_cast<double>(pentity->getScale())));
        stream.writeAttribute("sortorder", QString::number(pentity->sortOrder()));
        if ( pentity->tileMap() )
            stream.writeAttribute("tilemap", pentity->tileMap()->getName());
        stream.writeEndElement();
    }

    stream.writeEndElement();
    stream.writeEndElement();
    stream.writeEndDocument();
}
