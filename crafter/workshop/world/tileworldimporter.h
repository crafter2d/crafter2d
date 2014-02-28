#ifndef TILEWORLDIMPORTER_H
#define TILEWORLDIMPORTER_H

#include <QDir>

class QImage;
class QWidget;
class QString;
class QTileSet;
class TileMap;
class TileWorld;
class Layer;

class QTileWorldImporter
{
public:
    static TileWorld* import(QWidget *pparent, const QString& filename);

private:
    QTileWorldImporter(QWidget *pparent);

  // importing
    TileWorld*  importWorld(const QString& filename);
    TileWorld*  importOldFormat(const QString& filename);
    TileMap*    importMap(Layer& layer);
    QTileSet*   importTileSet(const QString &tileset);
    QString     importTexture(const QString& filename);

  // user handling
    QTileSet* loadTileSet(const QString& filename);
    QTileSet* selectTileSet(const QString &name);

  // data
    QWidget* mpParent;
    QDir     mBaseDir;
};

#endif // TILEWORLDIMPORTER_H
