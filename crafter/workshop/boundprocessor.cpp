#include "boundprocessor.h"

#include <core/world/bound.h>
#include <core/world/bounds.h>

#include "boundisland.h"

BoundProcessor::BoundProcessor()
{
}

// - Operations

void BoundProcessor::process(Bounds& bounds)
{
    cleanup();
    buildIslands(bounds);
    processIslands();
}

void BoundProcessor::cleanup()
{
    for ( int index = 0; index < mIslands.size(); ++index )
    {
        delete mIslands[index];
    }
    mIslands.clear();
}

void BoundProcessor::buildIslands(Bounds& bounds)
{
    for ( std::size_t index = 0; index < bounds.size(); ++index )
    {
        Bound* pbound = bounds[index];
        BoundIsland* pisland = findIsland(*pbound);
        if ( pisland == NULL )
        {
            pisland = new BoundIsland();
            mIslands.append(pisland);
        }

        Q_ASSERT(pisland);
        pisland->add(*pbound);
    }
}

void BoundProcessor::processIslands()
{
    BoundIsland* pisland = NULL;
    foreach (pisland, mIslands)
    {
        pisland->process();
    }
}

// - Searching

BoundIsland* BoundProcessor::findIsland(Bound& bound)
{
    BoundIsland* pisland = NULL;
    foreach (pisland, mIslands)
    {
        if ( pisland->belongsTo(bound) )
        {
            return pisland;
        }
    }
    return NULL;
}
