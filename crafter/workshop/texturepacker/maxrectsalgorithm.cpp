#include "maxrectsalgorithm.h"

#include <QImage>

#include "sprite.h"

MaxRectsAlgorithm::MaxRectsAlgorithm():
    mWidth(0),
    mHeight(0),
    mFreeRects()
{

}

MaxRectsAlgorithm::MaxRectsAlgorithm(int width, int height):
    mWidth(0),
    mHeight(0),
    mFreeRects()
{
    init(width, height);
}

void MaxRectsAlgorithm::init(int width, int height)
{
    mWidth = width;
    mHeight = height;

    QRect n(0, 0, width, height);

    mFreeRects.clear();
    mFreeRects.push_back(n);
}

void MaxRectsAlgorithm::insert(QVector<Sprite>& sprites, QVector<Sprite>& usedSprites)
{
    while ( !sprites.isEmpty() )
    {
        int bestScore1 = std::numeric_limits<int>::max();
        int bestScore2 = std::numeric_limits<int>::max();
        int bestSpriteIndex = -1;
        QRect bestNode;
        bool bestRotated;

        for ( int index = 0; index < sprites.size(); index++ )
        {
            Sprite& sprite = sprites[index];

            bool rotated;
            int score1, score2;
            QRect newNode = scoreRect(sprite.mOriginalSize, score1, score2, rotated);

            if ( score1 < bestScore1 || (score1 == bestScore1 && score2 < bestScore2) )
            {
                bestScore1 = score1;
                bestScore2 = score2;
                bestRotated = rotated;
                bestNode = newNode;
                bestSpriteIndex = index;
            }
        }

        if ( bestSpriteIndex == -1 )
        {
            // No image found that fits in this sprite sheet,
            // continue with the next sheet.
            return;
        }

        placeRect(bestNode);

        Sprite sprite = sprites[bestSpriteIndex];
        sprite.mCoordinates = bestNode;
        sprite.mRotated = bestRotated;
        usedSprites.push_back(sprite);

        sprites.remove(bestSpriteIndex);
    }
}

void MaxRectsAlgorithm::placeRect(const QRect& node)
{
    int numRectanglesToProcess = mFreeRects.size();
    for ( int index = 0; index < numRectanglesToProcess; ++index )
    {
        if ( splitFreeNode(mFreeRects[index], node) )
        {
            mFreeRects.remove(index);
            --index;
            --numRectanglesToProcess;
        }
    }

    pruneFreeList();
}

bool isContainedIn(const QRect &a, const QRect &b)
{
    return a.x() >= b.x()
        && a.y() >= b.y()
        && a.x()+a.width() <= b.x()+b.width()
        && a.y()+a.height() <= b.y()+b.height();
}

void MaxRectsAlgorithm::pruneFreeList()
{
    /// Go through each pair and remove any rectangle that is redundant.
    for( int i = 0; i < mFreeRects.size(); ++i )
    {
        for( int j = i+1; j < mFreeRects.size(); ++j )
        {
            if ( isContainedIn(mFreeRects[i], mFreeRects[j]) )
            {
                mFreeRects.erase(mFreeRects.begin()+i);
                --i;
                break;
            }
            if ( isContainedIn(mFreeRects[j], mFreeRects[i]) )
            {
                mFreeRects.erase(mFreeRects.begin()+j);
                --j;
            }
        }
    }
}

bool MaxRectsAlgorithm::splitFreeNode(const QRect& freeNode, const QRect& usedNode)
{
    if ( usedNode.x() >= freeNode.right()
      || usedNode.right() <= freeNode.x()
      || usedNode.y() >= freeNode.bottom()
      || usedNode.bottom() <= freeNode.y() )
    {
        return false;
    }

    if ( usedNode.x() < freeNode.right() && usedNode.right() > freeNode.x() )
    {
        // new node at the top side of the used node.
        if ( usedNode.y() > freeNode.y() && usedNode.y() < freeNode.bottom() )
        {
            QRect newNode = freeNode;
            newNode.setHeight(usedNode.y() - newNode.y() - 1);
            mFreeRects.push_back(newNode);
        }

        // new node at the bottom side of the used node.
        if ( usedNode.bottom() < freeNode.bottom() )
        {
            QRect newNode = freeNode;
            newNode.setY(usedNode.bottom() + 1);
            newNode.setHeight(freeNode.bottom() - usedNode.bottom());
            mFreeRects.push_back(newNode);
        }
    }

    if ( usedNode.y() < freeNode.bottom() && usedNode.bottom() > freeNode.y() )
    {
        // New node at the left side of the used node.
        if ( usedNode.x() > freeNode.x() && usedNode.x() < freeNode.right() )
        {
            QRect newNode = freeNode;
            newNode.setWidth(usedNode.x() - newNode.x() - 1);
            mFreeRects.push_back(newNode);
        }

        // New node at the right side of the used node
        if ( usedNode.right() < freeNode.right() )
        {
            QRect newNode = freeNode;
            newNode.setX(usedNode.right() + 1);
            newNode.setWidth(freeNode.right() - usedNode.right());
            mFreeRects.push_back(newNode);
        }
    }

    return true;
}

QRect MaxRectsAlgorithm::scoreRect(const QSize& size, int& score1, int& score2, bool& rotated)
{
    score1 = std::numeric_limits<int>::max();
    score2 = std::numeric_limits<int>::max();
    rotated = false;

    QRect newNode = findPositionForNewNodeBestShortSideFit(size, score1, score2, rotated);

    // cannot fit the current rectangle
    if ( newNode.height() == 0 )
    {
        score1 = std::numeric_limits<int>::max();
        score2 = std::numeric_limits<int>::max();
    }

    return newNode;
}

QRect MaxRectsAlgorithm::findPositionForNewNodeBestShortSideFit(const QSize& size, int& bestShortSideFit, int& bestLongSideFit, bool& rotated)
{
    QRect bestNode;

    bestShortSideFit = std::numeric_limits<int>::max();

    for ( QRect& rect : mFreeRects )
    {
        if ( rect.width() >= size.width() && rect.height() >= size.height() )
        {
            int leftoverHoriz = abs(rect.width() - size.width());
            int leftoverVert = abs(rect.height() - size.height());
            int shortSideFit = std::min(leftoverHoriz, leftoverVert);
            int longSideFit = std::max(leftoverHoriz, leftoverVert);

            if ( shortSideFit < bestShortSideFit
             || (shortSideFit == bestShortSideFit && longSideFit < bestLongSideFit) )
            {
                bestNode.setRect(rect.x(), rect.y(), size.width(), size.height());
                bestShortSideFit = shortSideFit;
                bestLongSideFit = longSideFit;
                rotated = false;
            }
        }

        if ( rect.width() >= size.height() && rect.height() >= size.width() )
        {
            int flippedLeftoverHoriz = abs(rect.width() - size.height());
            int flippedLeftoverVert = abs(rect.height() - size.width());
            int flippedShortSideFit = std::min(flippedLeftoverHoriz, flippedLeftoverVert);
            int flippedLongSideFit = std::max(flippedLeftoverHoriz, flippedLeftoverVert);

            if ( flippedShortSideFit < bestShortSideFit
              || (flippedShortSideFit == bestShortSideFit && flippedLongSideFit < bestLongSideFit) )
            {
                bestNode.setRect(rect.x(), rect.y(), size.height(), size.width());
                bestShortSideFit = flippedShortSideFit;
                bestLongSideFit = flippedLongSideFit;
                rotated = true;
            }
        }
    }
    return bestNode;
}
