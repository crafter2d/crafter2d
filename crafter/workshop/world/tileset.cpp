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
    QStringList filters;
    filters << "*.png";

    QDir dir(Project::getActiveProject().getFilePath(path));
    QStringList images = dir.entryList(filters, QDir::Files);
    std::sort(images.begin(), images.end(), compare);
    mTiles.resize(images.size());

    for ( int index = 0; index < images.size(); index++ )
    {
        mTiles[index].load(dir.absoluteFilePath(images[index]));
    }

    if ( !mTiles.empty() )
    {
        mTileSize = mTiles[0].size();
    }
}

// - Operations

void TileSet::paintTile(QPainter& painter, int index, int x, int y) const
{
    if ( index < mTiles.size() )
    {
        const QImage& tile = mTiles[index];
        painter.drawImage(x, y, tile);
    }
}
