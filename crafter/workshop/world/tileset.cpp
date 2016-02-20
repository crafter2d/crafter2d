#include "tileset.h"

#include <QDir>
#include <QImage>
#include <QPixmap>
#include <QPainter>

#include <qmath.h>

#include "../project.h"

TileSet::TileSet():
    Resource(Resource::eTileSet),
    mImagePath(),
    mTiles(),
    mTileSize()
{
}

// - Get/set

struct compareNames
{
    bool operator()(const QString& s1,const QString& s2)
    {
        if ( s1.length() < s2.length() )
        {
            return true;
        }
        else if ( s1.length() > s2.length() )
        {
            return false;
        }
        return s1.compare(s2) <= 0;
    }
} compare;

void TileSet::setImagePath(const QString& path)
{
    mImagePath = path;

    QStringList filters;
    filters << "*.png";

    QDir dir(Project::getActiveProject().getFilePath(path));
    QStringList images = dir.entryList(filters, QDir::Files);
    std::sort(images.begin(), images.end(), compare);

    auto& atlas = Project::getActiveProject().getSpriteAtlas();

    mTiles.clear();
    mTiles.resize(images.size());
    for ( int index = 0; index < images.size(); index++ )
    {
        QString name = dir.absoluteFilePath(images[index]);
        name = name.right(name.length() - Project::getActiveProject().getImagePath().length() - 1).toLower();

        Tile& tile = mTiles[index];
        tile.name = name;
        tile.spriteindex = atlas.lookup(name);
    }
}

void TileSet::determineTileSize()
{
    if ( !mTiles.empty() )
    {
        auto& atlas = Project::getActiveProject().getSpriteAtlas();
        mTileSize = atlas.bounds(mTiles[0].spriteindex).size();
    }
}

// - Operations

void TileSet::paintTile(QPainter& painter, const QPoint& pos, int index) const
{
    if ( index < mTiles.size() )
    {
        Project::getActiveProject().getSpriteAtlas().paint(painter, pos, mTiles[index].spriteindex);
        //const QImage& tile = mTiles[index].image;
        //painter.drawImage(x, y, tile);
    }
}
