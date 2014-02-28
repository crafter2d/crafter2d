#include "boundisland.h"

#include <math.h>
#include <core/world/bound.h>

static const int MAXDISTANCE = 3;

// static
bool BoundIsland::areConnected(const Bound& left, const Bound& right)
{
    return getConnectionSide(left, right) != BoundIsland::eNone;
}

BoundIsland::Side BoundIsland::getConnectionSide(const Bound& bound, const Bound& other)
{
    if ( bound.getLeft().distance(other.getLeft()) < MAXDISTANCE
      || bound.getLeft().distance(other.getRight()) < MAXDISTANCE )
    {
        return BoundIsland::eLeft;
    }
    else if ( bound.getRight().distance(other.getLeft()) < MAXDISTANCE
           || bound.getRight().distance(other.getRight()) < MAXDISTANCE )
    {
        return BoundIsland::eRight;
    }

    return BoundIsland::eNone;
}

BoundIsland::BoundIsland()
{
}

// - Query

bool BoundIsland::belongsTo(Bound& bound) const
{
    const Bound* pbound = findConnectedBound(bound);
    return pbound != NULL;
}

// - Maintenance

void BoundIsland::add(Bound& bound)
{
    const Bound* pbound = findConnectedBound(bound);
    if ( pbound != NULL )
    {
        // insert based on side of found bound

        Side side = getConnectionSide(*pbound, bound);
        Q_ASSERT(side != eNone);

        int index = mBounds.indexOf((Bound*)pbound) + (side == eLeft ? -1 : 1);
        mBounds.insert(index, &bound);
    }
    else
    {
        // first entry of this island
        mBounds.append(&bound);
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
        Bound* pb1 = mBounds[index];
        Bound* pb2 = mBounds[(index + 1) % mBounds.size()];
    }
}

void BoundIsland::createChain()
{
    Bound* pbound = NULL;
    foreach (pbound, mBounds)
    {
        // ensure that the left matches with right
        // b1.l b2.r b2.l b3.r ,etc
    }
}

// - Searching

const Bound* BoundIsland::findConnectedBound(const Bound& bound) const
{
    const Bound* pbound = NULL;
    foreach(pbound, mBounds)
    {
        if ( areConnected(*pbound, bound) )
        {
            return pbound;
        }
    }
    return NULL;
}

