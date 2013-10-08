#ifndef BOUNDPROCESSOR_H
#define BOUNDPROCESSOR_H

#include <QVector>

class Bound;
class Bounds;
class BoundIsland;

class BoundProcessor
{
public:
    BoundProcessor();

  // operations
    void process(Bounds& bounds);

private:
    typedef QVector<BoundIsland*> Islands;

  // operations
    void cleanup();
    void buildIslands(Bounds &bounds);
    void processIslands();

  // searching
    BoundIsland* findIsland(Bound& bound);

  // data
    Islands mIslands;
};

#endif // BOUNDPROCESSOR_H
