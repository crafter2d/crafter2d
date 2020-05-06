#include "projectdocument.h"

#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QXmlStreamReader>

#include "projectsettings.h"
#include "../script/scriptfile.h"
#include "../world/tileset.h"
#include "../world/tilesetreader.h"
#include "../world/tileworld.h"
#include "../world/tileworldreader.h"
#include "../world/tileentityreader.h"

Project *ProjectDocument::fromJson(QFile& file)
{    
    auto contents = file.readAll();
    QJsonParseError error;
    auto jsondoc = QJsonDocument::fromJson(contents, &error);

    QFileInfo info(file);
    auto pproject = std::make_unique<Project>();
    Project::setActiveProject(pproject.get());
    pproject->setBasePath(info.absolutePath());
    pproject->setFileName(info.absoluteFilePath());
    pproject->load();

    ProjectDocument doc(*pproject);
    if ( jsondoc.isNull() )
    {
        doc.loadOldFormat(contents);
    }
    else
    {
       doc.load(jsondoc.object());
    }

    return pproject.release();
}

ProjectDocument::ProjectDocument(Project &project):
    mProject(project)
{

}

QByteArray ProjectDocument::toJson() const
{
    mProject.saveDirtyResources();

    QJsonObject root;
    writeSettings(root);
    writeResources(root);
    QJsonDocument doc(root);
    return doc.toJson();
}

void ProjectDocument::writeSettings(QJsonObject& json) const
{
    auto& settings = mProject.getSettings();

    QJsonObject settingsobj;
    settingsobj["buildpaths"] = settings.getBuildPaths().join(';');

    json["settings"] = settingsobj;
}

void ProjectDocument::writeResources(QJsonObject &json) const
{
    QJsonObject resourceobj;

    QJsonArray scripts;
    for ( auto pscript : mProject.getScripts() )
    {
        scripts.append(pscript->getResourceName());
    }
    resourceobj["scripts"] = scripts;

    QJsonArray tilesets;
    for ( auto ptileset : mProject.getTileSets() )
    {
        tilesets.append(ptileset->getResourceName());
    }
    resourceobj["tilesets"] = tilesets;

    QJsonArray worlds;
    for ( auto pworld : mProject.getWorlds() )
    {
        worlds.append(pworld->getResourceName());
    }
    resourceobj["worlds"] = worlds;

    json["resources"] = resourceobj;
}

void ProjectDocument::load(const QJsonObject &json)
{
    if ( json.contains("settings") && json["settings"].isObject() ) {
        loadSettings(json["settings"].toObject());
    }

    if ( json.contains("resources") && json["resources"].isObject() ) {
        loadResources(json["resources"].toObject());
    }
}

void ProjectDocument::loadSettings(const QJsonObject &json)
{
    ProjectSettings settings;
    if ( json.contains("buildpaths") && json["buildpaths"].isString() ) {
        auto paths = json["buildpaths"].toString().split(';');
        settings.addBuildPaths(paths);
    }    
    mProject.setSettings(std::move(settings));
}

void ProjectDocument::loadResources(const QJsonObject &json)
{
    if ( json.contains("scripts") && json["scripts"].isArray() ) {
        auto array = json["scripts"].toArray();
        for ( auto obj : array ) {
            auto filename = obj.toString();
            loadScript(filename);
        }
    }

    // first tilesets as the worlds are depending on it
    if ( json.contains("tilesets") && json["tilesets"].isArray() ) {
        auto array = json["tilesets"].toArray();
        for ( auto obj : array) {
            auto filename = obj.toString();
            loadTileset(filename);
        }
    }

    if ( json.contains("worlds") && json["worlds"].isArray() ) {
        auto array = json["worlds"].toArray();
        for ( auto obj : array) {
            auto filename = obj.toString();
            loadWorld(filename);
        }
    }
}

void ProjectDocument::loadScript(const QString& fileName)
{
    QString path = QDir::toNativeSeparators(fileName);
    QString filepath = mProject.getBasePath() + QDir::separator() + path;
    ScriptFile* pscript = new ScriptFile(filepath);
    pscript->setResourceName(fileName);
    mProject.addScript(pscript);
}

void ProjectDocument::loadWorld(const QString& fileName)
{
    QString path = mProject.getBasePath() + QDir::separator() + fileName;
    QFile file(path);
    if ( file.open(QIODevice::ReadOnly) )
    {
        QTileWorldReader reader(file);
        TileWorld* pworld = reader.read();
        pworld->setResourceName(fileName);

        QFileInfo info(file);
        QString entityfilename = info.baseName() + ".xml";
        QFile entityfile(info.dir().filePath(entityfilename));
        TileEntityReader entityreader;
        entityreader.load(entityfile, *pworld);

        mProject.addWorld(pworld);
    }
}

void ProjectDocument::loadTileset(const QString& filename)
{
    QString path = mProject.getBasePath() + QDir::separator() + filename;
    QFile file(path);
    if ( file.open(QIODevice::ReadOnly) )
    {
        TileSetReader reader(file);
        TileSet* ptileset = reader.read();
        ptileset->setResourceName(filename);

        mProject.addTileSet(ptileset);
    }
}

void ProjectDocument::loadOldFormat(QByteArray& contents)
{
    QXmlStreamReader stream(contents);
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
                            mProject.setName(nameref.toString());
                        }
                    }
                    else if ( stream.name() == "script" )
                    {
                        QString path = QDir::toNativeSeparators(stream.readElementText());
                        loadScript(path);
                    }
                    else if ( stream.name() == "world" )
                    {
                        QString filepath = stream.readElementText();
                        loadWorld(filepath);
                    }
                    else if ( stream.name() == "tileset" )
                    {
                        QString filepath = stream.readElementText();
                        loadTileset(filepath);
                    }
                }
                break;
            default:
                break;
        }
    }
}

