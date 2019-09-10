#include "tileworld.h"

#include <limits>

#include <QFileInfo>
#include <QPainter>

#include "tileentity.h"
#include "tilemap.h"
#include "tilemapdesc.h"
#include "tilebound.h"
#include "tileset.h"

TileWorld::TileWorld(const TileWorldDesc& desc):
    Resource(Resource::eWorld),
    mDesc(desc),
    mMaps(),
    mBounds(),
    mpActiveMap(nullptr),
    mpSelectedBound(nullptr)
{
}

// - Get/set

const QString& TileWorld::getName() const
{
    return mDesc.name;
}

bool TileWorld::hasSelectedBound() const
{
    return mpSelectedBound != nullptr;
}

TileBound& TileWorld::getSelectedBound()
{
    Q_ASSERT(mpSelectedBound);
    return *mpSelectedBound;
}

void TileWorld::setSelectedBound(TileBound *pbound)
{
    if ( pbound != mpSelectedBound )
    {
        mpSelectedBound = pbound;
    }
}

const TileWorldDesc& TileWorld::getDesc() const
{
    return mDesc;
}

// - Query

QSize TileWorld::getMinimumSize() const
{
    QSize result;
    for ( int index = 0; index < mMaps.size(); ++index )
    {
        TileMap* pmap = mMaps[index];
        QSize minsize = pmap->getMinimumSize();
        if ( minsize.width() > result.width()
          || minsize.height() > result.height() )
        {
            result = minsize;
        }
    }
    return result;
}

bool TileWorld::hasActiveMap() const
{
    return mpActiveMap != nullptr;
}

TileMap* TileWorld::getActiveMap()
{
    return mpActiveMap;
}

void TileWorld::setActiveMap(TileMap& active)
{
    setActiveMap(&active);
}

void TileWorld::setActiveMap(TileMap* active)
{
    if ( active != mpActiveMap )
    {
        mpActiveMap = active;
        emit activeMapChanged(mpActiveMap);
    }
}

int TileWorld::getMapCount() const
{
    return mMaps.size();
}

const TileMap& TileWorld::getMap(int index) const
{
    return *mMaps[index];
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
    paintEntities(painter);
}

void TileWorld::paintMaps(QPainter& painter)
{
    for( auto pmap : mMaps)
    {
        pmap->paint(painter);
    }
}

void TileWorld::paintBounds(QPainter& painter)
{
    QPen pen(QBrush(QColor(255, 0, 255)), 2);

    painter.save();
    painter.setPen(pen);

    for ( auto pbound : mBounds )
    {
        painter.drawLine(pbound->left(), pbound->right());
    }

    if ( mpSelectedBound != nullptr )
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

    QPointF normal = mpSelectedBound->getNormal().toPointF();
    QPointF center = mpSelectedBound->getCenter();

    painter.drawLine(mpSelectedBound->left(), mpSelectedBound->right());
    painter.drawLine(center, center + (normal * 5));

    painter.setBrush(brush);
    painter.drawEllipse(mpSelectedBound->left(), 3, 3);
    painter.drawEllipse(mpSelectedBound->right(), 3, 3);
}

void TileWorld::paintEntities(QPainter& painter)
{
    for ( auto pentity : mEntities )
    {
        pentity->paint(painter);
    }
}

// - Maintenance

void TileWorld::addMap(TileMap* pmap)
{
    mMaps.append(pmap);

    connect(pmap, SIGNAL(mapChanged(TileMap&)), SLOT(on_mapChanged(TileMap&)));
}

void TileWorld::removeMap(TileMap& map)
{
    int index = mMaps.indexOf(&map);

    if ( mpActiveMap == &map )
    {
        // the map is currently active, change the active to the next or previous
        if ( index < mMaps.size() - 1 )
        {
            setActiveMap(mMaps[index + 1]);
        }
        else if ( index > 0 )
        {
            setActiveMap(mMaps[index - 1]);
        }
        else
        {
            setActiveMap(nullptr);
        }
    }

    mMaps.remove(index);
}

int TileWorld::getBoundCount() const
{
    return mBounds.size();
}

const TileBound& TileWorld::getBound(int index) const
{
    return *mBounds[index];
}

void TileWorld::addBound(TileBound* pbound)
{
    mBounds.append(pbound);

    setDirty(true);
}

TileBound &TileWorld::addBound(const QPoint& mousepos)
{
    TileBound* pbound = new TileBound(mousepos, mousepos);
    addBound(pbound);
    return *pbound;
}

void TileWorld::deleteSelectedBound()
{
    Q_ASSERT(mpSelectedBound != nullptr);
    TileBound* pbound = nullptr;
    std::swap(pbound, mpSelectedBound);
    int index = mBounds.indexOf(pbound);
    mBounds.remove(index);

    setDirty(true);
}

TileEntity& TileWorld::addEntity(Entity& entity)
{
    TileEntity* pentity = new TileEntity(*this, entity);
    mEntities.push_back(pentity);

    setDirty(true);

    return *mEntities.last();
}

int TileWorld::getTile(const QPoint& mousepos, TileField::Level level) const
{
    return mpActiveMap != nullptr ? mpActiveMap->getTile(mousepos, level) : TileSet::INVALID_TILE;
}

bool TileWorld::setTile(const QPoint& mousepos, TileField::Level level, int tile)
{
    if ( mpActiveMap != nullptr && mpActiveMap->setTile(mousepos, level, tile) )
    {
        emit worldDirty();
        return true;
    }
    return false;
}

void TileWorld::clearTile(const QPoint& mousepos, TileField::Level level)
{
    if ( mpActiveMap != nullptr )
    {
        mpActiveMap->clearTile(mousepos, level);
        emit worldDirty();
    }
}

void TileWorld::moveUp(int index)
{
    if ( index > 0 )
    {
        TileMap* pmap = mMaps[index];
        mMaps.remove(index);
        mMaps.insert(index - 1, pmap);
    }
}

void TileWorld::moveDown(int index)
{
    if ( index < mMaps.size() - 1 )
    {
        TileMap* pmap = mMaps[index];
        mMaps.remove(index);
        mMaps.insert(index + 1, pmap);
    }
}

void TileWorld::straightenBounds()
{
    for ( int index = 0; index < mBounds.size(); ++index )
    {
        TileBound* pbound = mBounds[index];
        pbound->straighten();
    }
}

// - Slots

void TileWorld::on_mapChanged(TileMap& /* map */)
{
    emit worldDirty();
}

// - Searching

TileBound* TileWorld::findBound(const QPoint& mousepos)
{
    TileBound* presult = nullptr;
    float nearest = 5.0f;

    for ( auto pbound : mBounds )
    {
        float distance;
        if ( pbound->hitTest(mousepos, distance) && distance < nearest )
        {
            presult = pbound;
            nearest = distance;
        }
    }

    return presult;
}

TileEntity* TileWorld::findEntity(const QPoint& mousepos)
{
    TileEntity* pbestEntity = nullptr;
    int closestDistance = std::numeric_limits<int>::max();
    for ( auto pentity : mEntities )
    {
        QRect bounds = pentity->getBoundingRect();
        if ( bounds.contains(mousepos) )
        {
            QPoint distance = bounds.topLeft() - mousepos;
            int length = distance.manhattanLength();
            if ( length < closestDistance  )
            {
                closestDistance = length;
                pbestEntity = pentity;
            }
        }
    }
    return pbestEntity;
}
