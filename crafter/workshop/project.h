#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QString>
#include <QVector>

class QDir;
class QTileSet;
class ScriptFile;
class TileWorld;

class Project : public QObject
{
    Q_OBJECT

public:
    typedef QVector<QTileSet*> TileSets;
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

    Worlds &getWorlds();
    Scripts& getScripts();

 // query
    int        getWorldCount() const;
    TileWorld &getWorld(int index);

    /// /brief Returns the full path of the file, relative from the project directory.
    QString getFilePath(const QString& file);

 // operations
    void create(QDir &path);

    void addWorld(TileWorld *pworld);
    void addTileSet(QTileSet *ptileset);

    bool load(const QString& fileName);
    void save();

    void build();

 // search
    QTileSet* lookupTileSet(const QString& name);

signals:
    void dataChanged();

private:
  // operations
    bool loadWorld(const QString& fileName);
    bool loadTileset(const QString& filename);

    void saveProjectResources();
    void saveProjectFile();

    bool generateScripts(QDir& path);

 // static data
    static Project* spActiveProject;

 // data
    QString  mName;
    QString  mFileName;
    QString  mBasePath;
    TileSets mTileSets;
    Worlds   mWorlds;
    Scripts  mScripts;
};

#endif // PROJECT_H
