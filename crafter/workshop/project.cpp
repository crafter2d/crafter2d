#include "project.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "script/scriptfile.h"
#include "world/tileworldreader.h"
#include "world/tileworldwriter.h"
#include "world/tilesetreader.h"
#include "world/tilesetwriter.h"
#include "world/tileset.h"
#include "tileworld.h"
#include "newprojectdialog.h"

// - Statics

Project* Project::spActiveProject = NULL;

Project* Project::createNew(QWidget* pparent)
{
    Project* pproject = NULL;

    NewProjectDialog dialog(pparent);
    if ( dialog.exec() == QDialog::Accepted )
    {
        QString projectname = dialog.getName();
        QDir projectpath(dialog.getPath());
        QString projectfile = projectpath.absoluteFilePath(projectname + ".craft");

        pproject = new Project();
        pproject->setName(projectname);
        pproject->setFileName(projectfile);
        pproject->create(projectpath);
    }

    return pproject;
}

Project& Project::getActiveProject()
{
    return *spActiveProject;
}

void Project::setActiveProject(Project *pproject)
{
    spActiveProject = pproject;
}

// - Project Implementation

Project::Project():
    mName(),
    mFileName(),
    mBasePath(),
    mTileSets(),
    mWorlds(),
    mScripts()
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

Project::Worlds& Project::getWorlds()
{
    return mWorlds;
}

Project::Scripts& Project::getScripts()
{
    return mScripts;
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

QString Project::getFilePath(const QString& file)
{
    return getFolder() + QDir::separator() + file;
}

// - Operations

void Project::create(QDir& path)
{
    // create the folders
    path.mkdir("effects");
    path.mkdir("images");
    path.mkdir("tilesets");
    path.mkdir("worlds");
}

void Project::addWorld(TileWorld* pworld)
{
    mWorlds.append(pworld);
    emit dataChanged();
}

void Project::addTileSet(QTileSet *ptileset)
{
    mTileSets.append(ptileset);
    emit dataChanged();
}

bool Project::loadWorld(const QString& fileName)
{
    QString path = mBasePath + QDir::separator() + fileName;
    QFile file(path);
    if ( file.open(QIODevice::ReadOnly) )
    {
        QTileWorldReader reader(file);
        TileWorld* pworld = reader.read();
        pworld->setResourceName(fileName);

        addWorld(pworld);

        return true;
    }

    return false;
}

bool Project::loadTileset(const QString& filename)
{
    QString path = mBasePath + QDir::separator() + filename;
    QFile file(path);
    if ( file.open(QIODevice::ReadOnly) )
    {
        TileSetReader reader(file);
        QTileSet* ptileset = reader.read();
        ptileset->setResourceName(filename);
        addTileSet(ptileset);
        return true;
    }

    return false;
}

bool Project::load(const QString &fileName)
{
    Project::setActiveProject(this);

    QFile file(fileName);
    file.open(QIODevice::ReadOnly);

    QXmlStreamReader stream;
    stream.setDevice(&file);

    QFileInfo info(file);
    mBasePath = info.absolutePath();

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
                    else if ( stream.name() == "script" )
                    {
                        QString path = stream.readElementText();
                        QString filepath = mBasePath + QDir::separator() + path;
                        ScriptFile* pscript = new ScriptFile(filepath);
                        pscript->setResourceName(path);
                        mScripts.append(pscript);
                    }
                    else if ( stream.name() == "world" )
                    {
                        QString filepath = stream.readElementText();
                        if ( !loadWorld(filepath) )
                        {
                            return false;
                        }
                    }
                    else if ( stream.name() == "tileset" )
                    {
                        QString filepath = stream.readElementText();
                        if ( !loadTileset(filepath) )
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
    saveProjectResources();
    saveProjectFile();
}

void Project::saveProjectResources()
{
    QDir path(getFolder());

    ScriptFile* pscript;
    foreach (pscript, mScripts)
    {
        if ( pscript->isDirty() )
        {
            pscript->save();
        }
    }

    QTileSet* ptileset;
    foreach (ptileset, mTileSets)
    {
        if ( ptileset->isDirty() )
        {
            QFile file(path.filePath(ptileset->getResourceName()));
            if ( file.open(QIODevice::WriteOnly | QIODevice::Text) )
            {
                TileSetWriter writer(file);
                writer.write(*ptileset);
            }
        }
    }

    TileWorld* pworld;
    foreach (pworld, mWorlds)
    {
        if ( pworld->isDirty() )
        {
            QFile file(path.filePath(pworld->getResourceName()));
            if ( file.open(QIODevice::WriteOnly) )
            {
                QTileWorldWriter writer(file);
                writer.write(*pworld);
            }
        }
    }
}

void Project::saveProjectFile()
{
    QFile file(mFileName);
    file.open(QIODevice::WriteOnly);

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    stream.writeStartElement("project");
    stream.writeAttribute("name", mName);

    ScriptFile* pscript;
    foreach (pscript, mScripts)
    {
        stream.writeTextElement("script", pscript->getResourceName());
    }

    QTileSet* ptileset;
    foreach (ptileset, mTileSets)
    {
        stream.writeTextElement("tileset", ptileset->getResourceName());
    }

    TileWorld* pworld;
    foreach (pworld, mWorlds)
    {
        stream.writeTextElement("world", pworld->getResourceName());
    }

    stream.writeEndElement();
    stream.writeEndDocument();
}

// - Building

void Project::build()
{
    QProcess compiler;

    QTileSet* ptileset;
    foreach (ptileset, mTileSets)
    {
        QString outputpath = "../compiled/tilesets/" + ptileset->getTileMap() + ".c2d";

        QStringList args;
        args.append(outputpath);

        compiler.start("compilerd.exe", args);
    }

    TileWorld* pworld;
    foreach (pworld, mWorlds)
    {
        QString outputpath = "../compiled/worlds/" + pworld->getName() + ".c2d";

        QStringList args;
        args.append(pworld->getResourceName());
        args.append(outputpath);

        compiler.start("compilerd.exe", args);
    }
}

// - Search

QTileSet* Project::lookupTileSet(const QString& name)
{
    QTileSet* pset = NULL;
    foreach (pset, mTileSets)
    {
        if ( pset->getResourceName().contains(name) )
        {
            return pset;
        }
    }
    return NULL;
}
