#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QString>
#include <QVector>

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

    const QString getBasePath() const;

    Entities& getEntities() { return mEntities; }
    Worlds &getWorlds();
    Scripts& getScripts();
    TileSets& getTileSets();

 // query
    int        getWorldCount() const;
    TileWorld &getWorld(int index);

    QString getFilePath(const Resource& resource) const;

    /// /brief Returns the full path of the file, relative from the project directory.
    QString getFilePath(const QString& file) const;

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
    ScriptFile* findScript(const QString& classname);

public slots:
    void on_messageReady(const QString& msg);

signals:
    void dataChanged();
    void messageAvailable(const QString& msg);

private:
  // operations
    bool loadWorld(const QString& fileName);
    bool loadTileset(const QString& filename);
    void loadObjects();

    void saveProjectResources();
    void saveProjectFile();

    bool generateScripts(QDir& path);

 // static data
    static Project* spActiveProject;

 // data
    QString  mName;
    QString  mFileName;
    QString  mBasePath;
    Entities mEntities;
    TileSets mTileSets;
    Worlds   mWorlds;
    Scripts  mScripts;
};

#endif // PROJECT_H
