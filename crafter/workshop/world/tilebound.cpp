#include "tilebound.h"

TileBound::TileBound():
    mLeft(),
    mRight()
{
}

TileBound::TileBound(const QPointF& left, const QPointF& right):
    mLeft(left),
    mRight(right)
{
    calculateNormal();
}

// get/set

const QPointF& TileBound::left() const
{
    return mLeft;
}

void TileBound::setLeft(const QPointF& pos)
{
    mLeft = pos;
    calculateNormal();
}

const QPointF& TileBound::right() const
{
    return mRight;
}

void TileBound::setRight(const QPointF& pos)
{
    mRight = pos;
    calculateNormal();
}

const QVector2D& TileBound::getNormal() const
{
    return mNormal;
}

// - Query

bool TileBound::hitTest(const QPointF& point, float& distance)
{
    float linelength = QVector2D(mLeft - mRight).length();

    float u = ( ( ( point.x() - mLeft.x() ) * ( mRight.x() - mLeft.x() ) ) +
                ( ( point.y() - mLeft.y() ) * ( mRight.y() - mLeft.y() ) ) ) /
            ( linelength * linelength );

    if ( u < -0.001f || u > 1.001f )
        return false;   // closest point does not fall within the line segment

    QVector2D intersection(mLeft + (mRight - mLeft) * u);
    distance = QVector2D(point).distanceToPoint(intersection);

    return true;
}

QPointF TileBound::getCenter() const
{
    return (mLeft + mRight) / 2;
}

// - Operations

void TileBound::calculateNormal()
{
    QPointF direction = mRight - mLeft;
    mNormal = QVector2D(-direction.y(), direction.x());
    mNormal.normalize();
}

void TileBound::straighten(float margin)
{
    if ( fabs(mLeft.x() - mRight.x()) < margin )
    {
        mRight.setX(mLeft.x());
    }
    else if ( fabs(mLeft.y() - mRight.y()) < margin )
    {
        mRight.setY(mLeft.y());
    }
}

void TileBound::flip()
{
    QPointF temp = mLeft;
    mLeft = mRight;
    mRight = temp;
    calculateNormal();
}
