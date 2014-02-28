#ifndef TILEBOUND_H
#define TILEBOUND_H

#include <QPoint>
#include <QVector2D>

class TileBound
{
public:
    TileBound();
    TileBound(const QPointF& left, const QPointF& right);

  // get/set
    const QPointF& left() const;
    void           setLeft(const QPointF& pos);

    const QPointF& right() const;
    void           setRight(const QPointF& pos);

    const QVector2D& getNormal() const;

  // query
    bool hitTest(const QPointF& pos, float& distance);
    QPointF getCenter() const;

  // operations
    void straighten(float margin = 3.0f);
    void flip();

private:

  // operations
    void calculateNormal();

  // data
    QPointF mLeft;
    QPointF mRight;
    QVector2D mNormal;
};

#endif // TILEBOUND_H
