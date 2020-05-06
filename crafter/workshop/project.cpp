#include "project.h"

#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QProcess>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "entity/entity.h"
#include "entity/entityreader.h"
#include "project/projectbuilder.h"
#include "project/projectmanager.h"
#include "project/projectrunner.h"
#include "script/scriptfile.h"
#include "world/tileworldreader.h"
#include "world/tileworldwriter.h"
#include "world/tilesetreader.h"
#include "world/tilesetwriter.h"
#include "world/tileset.h"
#include "world/tileworld.h"
#include "world/tileentityreader.h"
#include "world/tileentitywriter.h"

#include "newprojectdialog.h"

// - Statics

Project* Project::spActiveProject = nullptr;

Project* Project::createNew(QWidget* pparent)
{
    Project* pproject = nullptr;

    NewProjectDialog dialog(pparent);
    if ( dialog.exec() == QDialog::Accepted )
    {
        QString projectname = dialog.getName();
        QDir projectpath(dialog.getPath());

        int index = projectname.indexOf('.');
        if ( index >= 0 )
        {
            projectname = projectname.left(index);
        }

        pproject = new Project();
        pproject->setName(projectname);
        pproject->create(projectpath);
    }

    return pproject;
}

Project& Project::getActiveProject()
{
    Project* pactiveproject = ProjectManager::getInstance().getActiveProject();
    Q_ASSERT(pactiveproject != nullptr);
    return *pactiveproject;
}

void Project::setActiveProject(Project *pproject)
{
    ProjectManager::getInstance().setActiveProject(pproject);
}

// - Project Implementation

Project::Project():
    mSettings(),
    mName(),
    mFileName(),
    mBasePath(),
    mPaths(),
    mEntities(),
    mTileSets(),
    mWorlds(),
    mScripts(),
    mSpriteAtlas()
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

void Project::setBasePath(const QString& path)
{
    mBasePath = path;

    mPaths.clear();
    mPaths.append(mBasePath + "/images");
    mPaths.append(mBasePath + "/scripts");
    mPaths.append(mBasePath + "/tileatlas");
}

// - Query

QString Project::getFilePath(const Resource& resource) const
{
    return mBasePath + QDir::separator() + resource.getResourceName();
}

QString Project::getFilePath(const QString& file) const
{
    return mBasePath + QDir::separator() + file;
}

const QString& Project::getImagePath() const
{
    return mPaths[eImagePath];
}

const QString& Project::getScriptPath() const
{
    return mPaths[eScriptPath];
}

const QString& Project::getTileAtlasPath() const
{
    return mPaths[eTileAtlasPath];
}

QStringList Project::getFiles(const QString& path, const QStringList& filter)
{
    QStringList result;
    QString lookpath = getFilePath(path);
    QDir dir(lookpath);
    traverseDirectory(dir, filter, result);
    return result;
}

void Project::traverseDirectory(QDir& dir, const QStringList& filter, QStringList& result)
{
    QFileInfoList infos = dir.entryInfoList();
    for ( auto& info : infos )
    {
        if ( info.isDir() )
        {
            if ( info.fileName() != ".." && info.fileName() != "." )
            {
                dir.cd(info.fileName());
                traverseDirectory(dir, filter, result);
                dir.cdUp();
            }
        }
        else if ( filter.contains(info.suffix()) )
        {
            QString filePath = info.filePath();
            result.append(filePath.right(filePath.length() - mBasePath.length()));
        }
    }
}

// - Operations

void Project::create(QDir& path)
{
    setActiveProject(this);

    // create the project path
    if ( !path.exists(mName) )
    {
        path.mkdir(mName);
    }
    path.cd(mName);

    // determine the file path (in the root of the project directory)
    setBasePath(path.absolutePath());
    QString projectfile = path.absoluteFilePath(mName + ".craft");
    setFileName(projectfile);

    // create the folders
    path.mkdir("effects");
    path.mkdir("images");
    path.mkdir("objects");
    path.mkdir("scripts");
    path.mkdir("tileatlas");
    path.mkdir("tilesets");
    path.mkdir("worlds");

    if ( generateScripts(path) )
    {
        path.cd("scripts");
        path.cd(mName);

        QFileInfoList list = path.entryInfoList();
        for ( int i = 0; i < list.size(); ++i )
        {
            QFileInfo fileInfo = list.at(i);
            if ( fileInfo.isFile() )
            {
                QString filepath = path.absolutePath() + QDir::separator() + fileInfo.fileName();
                mScripts.append(new ScriptFile(filepath));
            }
        }
    }
}

bool Project::generateScripts(QDir& path)
{
    QProcess gen;
    gen.setWorkingDirectory(QDir::currentPath());

    QStringList arguments;
    arguments.append("project");
    arguments.append("name=" + mName);
    arguments.append("path=" + path.absolutePath());

#ifdef _DEBUG
    gen.start("gend.exe", arguments);
#else
    gen.start("gen.exe", arguments);
#endif

    if ( gen.waitForFinished() )
    {
        QString message;

        switch ( gen.exitStatus() )
        {
        case QProcess::CrashExit:
            message = "";
            break;

        case QProcess::NormalExit:
            if ( gen.exitCode() == 0 )
            {
                return true;
            }
            else
            {
                int exitcode = gen.exitCode();
                switch ( exitcode )
                {
                case -1:
                    message = "You should not see this message, it is a stupid mistake.";
                    break;
                case -2:
                    {
                        QByteArray error = gen.readAllStandardError();
                        if ( !error.isEmpty() )
                        {
                            message += "The following error occurred while creating the script files:\n" + QString(error);
                            break;
                        }
                        // else fall through and give general error message
                    }
                default:
                    message = "An unknown error has occurred while running the gen utility.";
                    break;
                }
            }
        }

        QMessageBox::critical(0, "Crafter Workshop", message, QMessageBox::Ok);
    }
    else
    {
        QMessageBox::critical(0, "Crafter Workshop", "Failed to launch the gen utility", QMessageBox::Ok);
    }

    return false;
}

void Project::addWorld(TileWorld* pworld)
{
    mWorlds.append(pworld);
    emit dataChanged();
}

void Project::addTileSet(TileSet *ptileset)
{
    mTileSets.append(ptileset);
    emit dataChanged();
}

void Project::addScript(ScriptFile* pscript)
{
    mScripts.append(pscript);
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

        QFileInfo info(file);
        QString entityfilename = info.baseName() + ".xml";
        QFile entityfile(info.dir().filePath(entityfilename));
        TileEntityReader entityreader;
        entityreader.load(entityfile, *pworld);

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
        TileSet* ptileset = reader.read();
        ptileset->setResourceName(filename);
        addTileSet(ptileset);
        return true;
    }

    return false;
}

void Project::loadObjects()
{
    QString path = mBasePath + QDir::separator() + "objects";
    QDirIterator it(path, QStringList() << "*.xml");
    while ( it.hasNext() )
    {
        QString filename = it.next();
        QFile file(filename);
        if ( file.open(QIODevice::ReadOnly) )
        {
            EntityReader reader(file);
            Entity* pentity = reader.read();
            if ( pentity != nullptr )
            {
                QString name = filename.right(filename.length() - mBasePath.length() - 1); // we don't need the slash
                pentity->setResourceName(name);
                mEntities.append(pentity);
            }
        }
    }
}

bool Project::load()
{
    Project::setActiveProject(this);

    mSpriteAtlas.load(getTileAtlasPath());

    loadObjects();

    return true;
}

void Project::saveDirtyResources()
{
    QDir path(mBasePath);

    ScriptFile* pscript;
    foreach (pscript, mScripts)
    {
        if ( pscript->isDirty() )
        {
            pscript->save();
        }
    }

    TileSet* ptileset;
    foreach (ptileset, mTileSets)
    {
        if ( ptileset->isDirty() )
        {
            TileSetWriter writer;
            writer.write(*ptileset);
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

            QFileInfo info(file);
            QString entityfilename = info.baseName() + ".xml";
            QFile entityfile(info.dir().filePath(entityfilename));
            TileEntityWriter entityWriter;
            entityWriter.write(entityfile, pworld->getEntities());
        }
    }
}

// - Building

void Project::build()
{
    ProjectBuilder* pbuilder = new ProjectBuilder(*this);
    connect(pbuilder, SIGNAL(messageAvailable(QString)), this, SLOT(on_messageReady(QString)));
    pbuilder->start();
}

// - Running

void Project::run()
{
    ProjectRunner* prunner = new ProjectRunner(*this);
    connect(prunner, SIGNAL(messageReady(QString)), this, SLOT(on_messageReady(QString)));
    prunner->start();
}

// - Search

TileSet* Project::lookupTileSet(const QString& name)
{
    for (auto ptileset : mTileSets)
    {
        if ( ptileset->getResourceName().contains(name) )
        {
            return ptileset;
        }
    }
    return nullptr;
}

Entity* Project::lookupEntity(const QString& name)
{
    for ( auto pentity : mEntities )
    {
        if ( pentity->getResourceName() == name )
            return pentity;
    }
    return nullptr;
}

ScriptFile* Project::findScript(const QString& classname)
{
    QString filename = classname;
    filename = tr("scripts") + QDir::separator() + filename.replace('.', QDir::separator()) + ".as";

    for (auto pfile : mScripts)
    {
        if ( pfile->getResourceName() == filename )
        {
            return pfile;
        }
    }
    return nullptr;
}

// - Slots

void Project::on_messageReady(const QString& msg)
{
    // pass on to those that are interested
    emit messageAvailable(msg);
}
