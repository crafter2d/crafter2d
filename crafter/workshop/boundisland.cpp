#include "boundisland.h"

#include <math.h>

#include "world/tilebound.h"

static const int MAXDISTANCE = 3;

// static
bool BoundIsland::areConnected(const TileBound& left, const TileBound& right)
{
    return getConnectionSide(left, right) != BoundIsland::eNone;
}

BoundIsland::Side BoundIsland::getConnectionSide(const TileBound& bound, const TileBound& other)
{
    if ( (bound.left() - other.left()).manhattanLength() < MAXDISTANCE
      || (bound.left() - other.right()).manhattanLength() < MAXDISTANCE )
    {
        return BoundIsland::eLeft;
    }
    else if ( (bound.right() - other.left()).manhattanLength() < MAXDISTANCE
           || (bound.right() - other.right()).manhattanLength() < MAXDISTANCE )
    {
        return BoundIsland::eRight;
    }

    return BoundIsland::eNone;
}

BoundIsland::BoundIsland()
{
}

// - Query

bool BoundIsland::belongsTo(TileBound& bound) const
{
    const TileBound* pbound = findConnectedBound(bound);
    return pbound != NULL;
}

// - Maintenance

void BoundIsland::add(TileBound& bound)
{
    const TileBound* pbound = findConnectedBound(bound);
    if ( pbound != NULL )
    {
        // insert based on side of found bound

        Side side = getConnectionSide(*pbound, bound);
        Q_ASSERT(side != eNone);

        int index = 0;//  mBounds.indexOf(*pbound) + (side == eLeft ? -1 : 1);
        mBounds.insert(index, bound);
    }
    else
    {
        // first entry of this island
        mBounds.append(bound);
    }
}

// - Operations

void BoundIsland::process()
{
    if ( mBounds.size() <= 1 )
    {
        return;
    }

    createChain();

    for ( int index = 0; index < mBounds.size(); ++index )
    {
        TileBound& pb1 = mBounds[index];
        TileBound& pb2 = mBounds[(index + 1) % mBounds.size()];
    }
}

void BoundIsland::createChain()
{
    //TileBound* pbound = NULL;
    //foreach (pbound, mBounds)
    for ( auto& bound : mBounds )
    {
        // ensure that the left matches with right
        // b1.l b2.r b2.l b3.r ,etc
    }
}

// - Searching

const TileBound *BoundIsland::findConnectedBound(const TileBound &findbound) const
{
    const TileBound* pbound = NULL;
    for( auto& bound : mBounds )
    {
        if ( areConnected(bound, findbound) )
        {
            return pbound;
        }
    }
    return NULL;
}

