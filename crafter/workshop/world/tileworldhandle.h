#ifndef TILEWORLDHANDLE_H
#define TILEWORLDHANDLE_H

#include <QObject>

class TileWorld;

class TileWorldHandle : public QObject
{
    Q_OBJECT

public:
    TileWorldHandle();
    TileWorldHandle(const TileWorldHandle& that);
    TileWorldHandle(TileWorld& world);

    void operator=(const TileWorldHandle& that);

  // operations
    TileWorld& operator*();
    TileWorld* operator->();

private:

  // data
    TileWorld* mpWorld;
};

Q_DECLARE_METATYPE(TileWorldHandle)

#endif // TILEWORLDHANDLE_H
