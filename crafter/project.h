#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QString>
#include <QVector>

class TileWorld;

class Project : public QObject
{
    Q_OBJECT

public:
    typedef QVector<TileWorld*> Worlds;

    Project();

 // get/set
    const QString& getName() const;
    void           setName(const QString& name);

    const QString& getFileName() const;
    void           setFileName(const QString& name);

    const QString getFolder() const;

 // query
    QString getFilename() const;

    int        getWorldCount() const;
    TileWorld &getWorld(int index);

 // operations
    void addWorld(TileWorld *pworld);
    bool load(const QString& fileName);
    void save();

 // fixing
    void fixMaps();

signals:
    void dataChanged();

private:
  // operations
    bool loadWorld(const QString& fileName);

 // data
    QString mName;
    QString mFileName;
    Worlds  mWorlds;
};

#endif // PROJECT_H
