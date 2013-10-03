#include "project.h"

#include <QFile>
#include <QFileInfo>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include <core/smartptr/autoptr.h>
#include <core/string/string.h>

#include <engine/world/layer.h>
#include <engine/world/world.h>
#include <engine/world/worldreader.h>

#include "tileworld.h"

Project::Project():
    mName(),
    mFileName(),
    mWorlds()
{
}

// - Get/set

const QString& Project::getName() const
{
    return mName;
}

void Project::setName(const QString &name)
{
    mName = name;
}

const QString& Project::getFileName() const
{
    return mFileName;
}

void Project::setFileName(const QString& name)
{
    mFileName = name;
}

const QString Project::getFolder() const
{
    return QFileInfo(mFileName).absolutePath();
}

// - Query

int Project::getWorldCount() const
{
    return mWorlds.size();
}

TileWorld& Project::getWorld(int index)
{
    return *mWorlds[index];
}

// - Operations

void Project::addWorld(TileWorld* pworld)
{
    mWorlds.append(pworld);
    emit dataChanged();
}

bool Project::loadWorld(const QString& fileName)
{
    String path = String::fromUtf8(fileName.toUtf8().data());

    AutoPtr<World> world = new World();

    WorldReader reader;
    if ( !reader.read(*world, path) )
    {
        return false;
    }

    TileWorld* pworld = TileWorld::fromWorld(world.release());
    addWorld(pworld);

    return true;
}

bool Project::load(const QString &fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);

    QXmlStreamReader stream;
    stream.setDevice(&file);

    setFileName(fileName);

    while ( !stream.atEnd() )
    {
        QXmlStreamReader::TokenType type = stream.readNext();
        switch ( type )
        {
            case QXmlStreamReader::StartElement:
                {
                    if ( stream.name() == "project" )
                    {
                        QStringRef nameref = stream.attributes().value("name");
                        if ( !nameref.isEmpty() )
                        {
                            setName(nameref.toString());
                        }
                    }
                    else if ( stream.name() == "world" )
                    {
                        QString filepath = stream.readElementText();
                        if ( !loadWorld(filepath) )
                        {
                            return false;
                        }
                    }
                }
                break;
            default:
                break;
        }
    }

    return true;
}

void Project::save()
{
    QFile file(mFileName);
    file.open(QIODevice::WriteOnly);

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("project");
    stream.writeAttribute("name", mName);

    TileWorld* pworld;
    foreach (pworld, mWorlds)
    {
        stream.writeTextElement("world", pworld->getFileName());

        pworld->save();
    }

    stream.writeEndElement();
    stream.writeEndDocument();
}

// - Fixing code

// In some stage, a bug has been brought into live where index 0 it not tile 1! This
// function will fix broken maps by substracting 1 of all tiles. Only -1 should mean that
// there is no tile there.
void Project::fixMaps()
{
    TileWorld* pworld;
    foreach (pworld, mWorlds)
    {
        pworld->fixMaps();
    }
}
