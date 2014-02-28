#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QString>
#include <QVector>

class QDir;
class QTileSet;
class TileWorld;

class Project : public QObject
{
    Q_OBJECT

public:
    typedef QVector<QTileSet*> TileSets;
    typedef QVector<TileWorld*> Worlds;

    static Project* createNew(QWidget *pparent = 0);

    static Project& getActiveProject();
    static void     setActiveProject(Project *pproject);

    Project();

 // get/set
    const QString& getName() const;
    void           setName(const QString& name);

    const QString& getFileName() const;
    void           setFileName(const QString& name);

    const QString getFolder() const;

    Worlds &getWorlds();

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

 // static data
    static Project* spActiveProject;

 // data
    QString  mName;
    QString  mFileName;
    QString  mBasePath;
    TileSets mTileSets;
    Worlds   mWorlds;
};

#endif // PROJECT_H
