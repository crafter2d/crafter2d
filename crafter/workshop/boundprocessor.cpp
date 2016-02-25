#include "boundprocessor.h"

#include "world/tilebound.h"
#include "boundisland.h"

BoundProcessor::BoundProcessor():
    mIslands()
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
    for ( auto& bound : bounds )
    {
        BoundIsland* pisland = findIsland(bound);
        if ( pisland == NULL )
        {
            pisland = new BoundIsland();
            mIslands.append(pisland);
        }

        Q_ASSERT(pisland);
        pisland->add(bound);
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

BoundIsland* BoundProcessor::findIsland(TileBound& bound)
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
