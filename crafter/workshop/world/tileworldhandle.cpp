#include "tileworldhandle.h"

#include <QFileInfo>

#include "tileworld.h"

TileWorldHandle::TileWorldHandle():
    ResourceHandle(),
    mpWorld(0)
{
}

TileWorldHandle::TileWorldHandle(const TileWorldHandle& that):
    ResourceHandle(),
    mpWorld(that.mpWorld)
{
}

TileWorldHandle::TileWorldHandle(TileWorld &world):
    ResourceHandle(world),
    mpWorld(&world)
{
}

void TileWorldHandle::operator =(const TileWorldHandle& that)
{
    mpWorld = that.mpWorld;
}

bool TileWorldHandle::operator==(const TileWorld& that) const
{
    return mpWorld == &that;
}

// - Operations

TileWorld& TileWorldHandle::operator*()
{
    return *mpWorld;
}

TileWorld* TileWorldHandle::operator->()
{
    return mpWorld;
}
