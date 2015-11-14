#ifndef TILEWORLDHANDLE_H
#define TILEWORLDHANDLE_H

#include "resourcehandle.h"

class TileWorld;

class TileWorldHandle : public ResourceHandle
{
    Q_OBJECT

public:
    TileWorldHandle();
    TileWorldHandle(const TileWorldHandle& that);
    TileWorldHandle(TileWorld& world);

    void operator=(const TileWorldHandle& that);
    bool operator==(const TileWorld& that) const;

  // operations
    TileWorld& operator*();
    TileWorld* operator->();

private:

  // data
    TileWorld* mpWorld;
};

Q_DECLARE_METATYPE(TileWorldHandle)

#endif // TILEWORLDHANDLE_H
