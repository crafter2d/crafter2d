#ifndef BOUNDPROCESSOR_H
#define BOUNDPROCESSOR_H

#include <QVector>

class TileBound;
class BoundIsland;

class BoundProcessor
{
public:
    using Bounds = QVector<TileBound>;

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
    BoundIsland* findIsland(TileBound &bound);

  // data
    Islands mIslands;
};

#endif // BOUNDPROCESSOR_H
