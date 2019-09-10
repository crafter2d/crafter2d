#include "tileentityreader.h"

#include <QFile>
#include <QXmlStreamReader>

#include "world/tileentity.h"
#include "world/tileworld.h"

#include "project.h"

TileEntityReader::TileEntityReader()
{

}

void TileEntityReader::load(QFile& input, TileWorld& world)
{
    if ( !input.open(QIODevice::ReadOnly) )
        return;

    QXmlStreamReader stream;
    stream.setDevice(&input);

    while ( !stream.atEnd() )
    {
        QXmlStreamReader::TokenType type = stream.readNext();
        switch ( type )
        {
            case QXmlStreamReader::StartElement:
                {
                    if ( stream.name() == "entity" )
                    {
                        loadEntity(stream, world);
                    }
                }
                break;
        }
    }
}

void TileEntityReader::loadEntity(QXmlStreamReader& stream, TileWorld& world)
{
    QString name = stream.attributes().value("name").toString();
    auto* pentity = Project::getActiveProject().lookupEntity(name);
    if ( pentity )
    {
        // the engine takes the center of the object as position, though the workshop takes the upper left corner
        QPoint pos(stream.attributes().value("x").toInt(), stream.attributes().value("y").toInt());
        auto& entity = world.addEntity(*pentity);
        entity.setCenterPosition(pos);
        entity.setScale(stream.attributes().value("scale").toFloat());
    }
}
