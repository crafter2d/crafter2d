#ifndef BOUNDISLAND_H
#define BOUNDISLAND_H

#include <QVector>

class TileBound;

class BoundIsland
{
public:
    enum Side { eLeft, eRight, eNone };

    BoundIsland();

  // query
    bool belongsTo(TileBound& bound) const;

  // maintenance
    void add(TileBound& bound);

  // operations
    void process();

private:
    typedef QVector<TileBound> Bounds;

  // statics
    static Side getConnectionSide(const TileBound& bound, const TileBound& other);
    static bool areConnected(const TileBound& left, const TileBound& right);

  // operations
    void createChain();

  // searching
    const TileBound* findConnectedBound(const TileBound &bound) const;

  // bounds
    Bounds mBounds;
};

#endif // BOUNDISLAND_H
