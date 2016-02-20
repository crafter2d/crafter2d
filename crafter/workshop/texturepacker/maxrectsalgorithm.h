#ifndef MAXRECTSALGORITHM_H
#define MAXRECTSALGORITHM_H

#include <QRect>
#include <QVector>

class QImage;

class Sprite;

// Implementation from:
// http://clb.demon.fi/projects/even-more-rectangle-bin-packing

class MaxRectsAlgorithm
{
public:
    MaxRectsAlgorithm();
    MaxRectsAlgorithm(int width, int height);

    void init(int width, int height);

    void insert(QVector<Sprite> &sprites, QVector<Sprite> &usedSprites);

private:
    using Rects = QVector<QRect>;

  // operations
    void placeRect(const QRect& node);
    QRect scoreRect(const QSize& size, int& score1, int& score2, bool &rotated);
    bool splitFreeNode(const QRect& freeNode, const QRect& usedNode);
    void pruneFreeList();

    QRect findPositionForNewNodeBestShortSideFit(const QSize& size, int& bestShortSideFit, int& bestLongSideFit, bool &rotated);

  // data
    int mWidth;
    int mHeight;
    Rects mFreeRects;
};

#endif // MAXRECTSALGORITHM_H
