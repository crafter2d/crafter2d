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

static bool is_between (float x, float bound1, float bound2, float tolerance)
{
   // Handles cases when 'bound1' is greater than 'bound2' and when
   // 'bound2' is greater than 'bound1'.
   return (((x >= (bound1 - tolerance)) && (x <= (bound2 + tolerance))) ||
      ((x >= (bound2 - tolerance)) && (x <= (bound1 + tolerance))));
}

bool TileBound::hitTest(const QPointF& point, float& distance)
{
    if ( is_between(point.x(), mLeft.x(), mRight.x(), 3.001f) && is_between(point.y(), mLeft.y(), mRight.y(), 3.001f) )
    {
        float A = mLeft.y() - mRight.y();
        float B = mRight.x() - mLeft.x();
        float C = mLeft.x() * mRight.y() - mRight.x() * mLeft.y();

        float dist = std::abs(A*point.x() + B*point.y() + C) / std::sqrt(A * A + B * B);

        if ( dist <= 3.001f )
        {
            distance = dist;
            return true;
        }
    }

    return false;
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
