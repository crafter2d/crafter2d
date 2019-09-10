#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QString>
#include <QVector>

#include "texturepacker/spriteatlas.h"

class QDir;
class Entity;
class Resource;
class ScriptFile;
class TileSet;
class TileWorld;

class Project : public QObject
{
    Q_OBJECT

public:
    using Entities = QVector<Entity*>;
    typedef QVector<TileSet*> TileSets;
    typedef QVector<TileWorld*> Worlds;
    typedef QVector<ScriptFile*> Scripts;

    static Project* createNew(QWidget *pparent = 0);

    static Project& getActiveProject();
    static void     setActiveProject(Project *pproject);

    Project();

 // get/set
    const QString& getName() const;
    void           setName(const QString& name);

    const QString& getFileName() const;
    void           setFileName(const QString& name);

    const QString getBasePath() const {
        return mBasePath;
    }

    Entities& getEntities() { return mEntities; }
    Worlds &getWorlds() { return mWorlds; }
    Scripts& getScripts() { return mScripts; }
    TileSets& getTileSets() { return mTileSets; }
    SpriteAtlas& getSpriteAtlas() { return mSpriteAtlas; }

 // query
    QString getFilePath(const Resource& resource) const;

    /// /brief Returns the full path of the file, relative from the project directory.
    QString getFilePath(const QString& file) const;

    /// Returns all files from the given basepath\path
    /// filter : a list of all accepted extensions
    QStringList getFiles(const QString& path, const QStringList &filter);

    const QString& getImagePath() const;
    const QString& getScriptPath() const;
    const QString& getTileAtlasPath() const;


 // operations
    void create(QDir &path);

    void addWorld(TileWorld *pworld);
    void addTileSet(TileSet *ptileset);
    void addScript(ScriptFile* pscript);

    bool load(const QString& fileName);
    void save();

    void build();

    void run();

 // search
    TileSet* lookupTileSet(const QString& name);
    Entity* lookupEntity(const QString& name);
    ScriptFile* findScript(const QString& classname);

public slots:
    void on_messageReady(const QString& msg);

signals:
    void dataChanged();
    void messageAvailable(const QString& msg);

private:
    enum PathName { eImagePath, eScriptPath, eTileAtlasPath, ePathCount };
  // get/set
    void setBasePath(const QString& path);

  // operations
    bool loadWorld(const QString& fileName);
    bool loadTileset(const QString& filename);
    void loadObjects();

    void saveProjectResources();
    void saveProjectFile();

    void traverseDirectory(QDir& dir, const QStringList& filter, QStringList &result);

    bool generateScripts(QDir& path);

 // static data
    static Project* spActiveProject;

 // data
    QString  mName;
    QString  mFileName;
    QString  mBasePath;
    QStringList mPaths;
    Entities mEntities;
    TileSets mTileSets;
    Worlds   mWorlds;
    Scripts  mScripts;
    SpriteAtlas mSpriteAtlas;
};

#endif // PROJECT_H
