#include "tileworld.h"

#include <QFileInfo>
#include <QPainter>

#include <engine/world/bound.h>
#include <engine/world/bounds.h>
#include <engine/world/world.h>
#include <engine/world/worldwriter.h>
#include <engine/world/layer.h>

#include "stringinterface.h"
#include "tile.h"
#include "tilemap.h"

// static
TileWorld* TileWorld::fromWorld(World* pworld)
{
    TileWorld* ptileworld = new TileWorld(pworld);

    QString filename = toQString(pworld->getFilename());
    QString path = QFileInfo(filename).absolutePath();
    for ( int index = 0; index < pworld->getLayerCount(); ++index )
    {
        Layer* player = pworld->getLayer(index);

        TileMap* pmap = new TileMap(*ptileworld, player);
        pmap->load(path);

        ptileworld->addMap(pmap);
    }

    if ( ptileworld->getMapCount() > 0 )
    {
        ptileworld->setActiveMap(ptileworld->getMap(0));
    }

    return ptileworld;
}

TileWorld::TileWorld():
    QObject(NULL),
    mpWorld(NULL),
    mMaps(),
    mpActiveMap(NULL),
    mpSelectedBound(NULL)
{
}

TileWorld::TileWorld(World* pworld):
    QObject(NULL),
    mpWorld(pworld),
    mMaps(),
    mpActiveMap(NULL),
    mpSelectedBound(NULL)
{
}

// - Get/set

bool TileWorld::hasSelectedBound() const
{
    return mpSelectedBound != NULL;
}

Bound& TileWorld::getSelectedBound()
{
    Q_ASSERT(mpSelectedBound);
    return *mpSelectedBound;
}

void TileWorld::setSelectedBound(Bound* pbound)
{
    if ( pbound != mpSelectedBound )
    {
        mpSelectedBound = pbound;
    }
}

// - Query

QString TileWorld::getName() const
{
    QString result = mpWorld->getName().toUtf8().c_str();
    return result;
}

QString TileWorld::getFileName() const
{
    QString result = mpWorld->getFilename().toUtf8().c_str();
    return result;
}

void TileWorld::setFileName(const QString& filename)
{
    mpWorld->setFilename(String::fromUtf8(filename.toUtf8().data()));
}

QSize TileWorld::getMinimumSize() const
{
    QSize result;
    for ( int index = 0; index < mMaps.size(); ++index )
    {
        TileMap* pmap = mMaps[index];
        QSize minsize = pmap->getSize();
        if ( minsize.width() > result.width()
          || minsize.height() > result.height() )
        {
            result = minsize;
        }
    }
    return result;
}

TileMap& TileWorld::getActiveMap()
{
    return *mpActiveMap;
}

void TileWorld::setActiveMap(TileMap& active)
{
    if ( &active != mpActiveMap )
    {
        mpActiveMap = &active;
        emit activeMapChanged(*mpActiveMap);
    }
}

int TileWorld::getMapCount() const
{
    return mMaps.size();
}

TileMap& TileWorld::getMap(int index)
{
    return *mMaps[index];
}

// - Painting

void TileWorld::paint(QPainter& painter)
{
    paintMaps(painter);
    paintBounds(painter);
}

void TileWorld::paintMaps(QPainter& painter)
{
    TileMap* pmap = NULL;
    foreach(pmap, mMaps)
    {
        pmap->paint(painter);
    }
}

void TileWorld::paintBounds(QPainter& painter)
{
    QPen pen(QBrush(QColor(255, 0, 255)), 2);

    painter.save();
    painter.setPen(pen);

    const Bounds& bounds = mpWorld->getBounds();
    for ( std::size_t index = 0; index < bounds.size(); ++index )
    {
        const Bound& bound = *bounds[index];
        const Vector& left = bound.getLeft();
        const Vector& right = bound.getRight();

        painter.drawLine(QPointF(left.x, left.y), QPointF(right.x, right.y));
    }

    if ( mpSelectedBound != NULL )
    {
        paintSelectedBound(painter);
    }

    painter.restore();
}

void TileWorld::paintSelectedBound(QPainter& painter)
{
    QBrush brush(QColor(0, 0, 255));
    QPen pen(brush, 2);
    painter.setPen(pen);

    const Vector& left = mpSelectedBound->getLeft();
    const Vector& right = mpSelectedBound->getRight();
    Vector vnormal = mpSelectedBound->getNormal();

    QPointF posleft(left.x, left.y);
    QPointF posright(right.x, right.y);
    QPointF center = (posleft + posright) / 2;
    QPointF normal(vnormal.x, vnormal.y);

    painter.drawLine(posleft, posright);
    painter.drawLine(center, center + (normal * 5));

    painter.setBrush(brush);
    painter.drawEllipse(posleft, 3, 3);
    painter.drawEllipse(posright, 3, 3);
}

// - Maintenance

void TileWorld::addMap(TileMap* pmap)
{
    mMaps.append(pmap);
}

void TileWorld::addMap(LayerDefinition* pdefinition)
{
    Layer* player = mpWorld->createLayer();
    player->create(pdefinition);
    mpWorld->addLayer(player);

    TileMap* pmap = new TileMap(*this, player);
    addMap(pmap);
}

Bound& TileWorld::addBound(const QPoint& mousepos)
{
    Vector pos(mousepos.x(), mousepos.y());
    return mpWorld->addBound(pos, pos);
}

Tile TileWorld::getTile(const QPoint& mousepos, LayerLevel level)
{
    if ( mpActiveMap != NULL )
    {
        return mpActiveMap->getTile(mousepos, level);
    }

    return Tile();
}

bool TileWorld::setTile(const QPoint& mousepos, LayerLevel level, const Tile& tile)
{
    bool result = false;
    if ( mpActiveMap != NULL )
    {
        result = mpActiveMap->setTile(mousepos, level, tile);
    }
    return result;
}

void TileWorld::clearTile(const QPoint& mousepos, LayerLevel level)
{
    if ( mpActiveMap != NULL )
    {
        mpActiveMap->clearTile(mousepos, level);
    }
}

void TileWorld::save()
{
    if ( mpWorld != NULL )
    {
        WorldWriter writer;
        writer.write(*mpWorld, mpWorld->getFilename());
    }
}

void TileWorld::moveUp(int index)
{
    if ( index > 0 )
    {
        TileMap* pmap = mMaps[index];
        mMaps.remove(index);
        mMaps.insert(index - 1, pmap);

        mpWorld->moveLayer(&pmap->getLayer(), -1);
    }
}

void TileWorld::moveDown(int index)
{
    if ( index < mMaps.size() - 1 )
    {
        TileMap* pmap = mMaps[index];
        mMaps.remove(index);
        mMaps.insert(index + 1, pmap);

        mpWorld->moveLayer(&pmap->getLayer(), 1);
    }
}

void TileWorld::straightenBounds()
{
    Bounds& bounds = mpWorld->getBounds();
    for ( std::size_t index = 0; index < bounds.size(); ++index )
    {
        Bound* pbound = bounds[index];
        pbound->straighten();
    }
}

// - Slots

void TileWorld::mapChanged(TileMap& /* map */)
{
    emit worldDirty();
}

// - Searching

Bound* TileWorld::findBound(const QPoint& mousepos)
{
    Bound* presult;
    float nearest = 5.0f;

    Vector pos(mousepos.x(), mousepos.y());
    for ( int index = 0; index < mpWorld->getBoundCount(); ++index )
    {
        float distance;
        Bound& bound = mpWorld->getBound(index);
        if ( bound.hitTest(pos, distance) && distance < nearest )
        {
            presult = &bound;
            nearest = distance;
        }
    }

    return presult;
}

// - Fixing
// See TileWorld::fixMaps for explanation

void TileWorld::fixMaps()
{
    TileMap* pmap;
    foreach (pmap, mMaps)
    {
        pmap->fix();
    }
}
