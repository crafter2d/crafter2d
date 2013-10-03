#ifndef BOUNDISLAND_H
#define BOUNDISLAND_H

#include <QVector>

class Bound;

class BoundIsland
{
public:
    enum Side { eLeft, eRight, eNone };

    BoundIsland();

  // query
    bool belongsTo(Bound& bound) const;

  // maintenance
    void add(Bound& bound);

  // operations
    void process();

private:
    typedef QVector<Bound*> Bounds;

  // statics
    static Side getConnectionSide(const Bound& bound, const Bound& other);
    static bool areConnected(const Bound& left, const Bound& right);

  // operations
    void createChain();

  // searching
    const Bound* findConnectedBound(const Bound &bound) const;

  // bounds
    Bounds mBounds;
};

#endif // BOUNDISLAND_H
