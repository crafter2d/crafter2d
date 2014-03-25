#include "tileworldhandle.h"

TileWorldHandle::TileWorldHandle():
    QObject(0),
    mpWorld(0)
{
}

TileWorldHandle::TileWorldHandle(const TileWorldHandle& that):
    QObject(0),
    mpWorld(that.mpWorld)
{
}

TileWorldHandle::TileWorldHandle(TileWorld &world):
    QObject(NULL),
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
