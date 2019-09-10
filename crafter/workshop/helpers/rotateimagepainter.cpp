#include "rotateimagepainter.h"

#include <QPainter>

const QRect RotateImagePainter::sEmptyRect(0, 0, -1, -1);

RotateImagePainter::RotateImagePainter(QPainter& painter):
    mPainter(painter)
{
    mPainter.save();
}

RotateImagePainter::~RotateImagePainter()
{
    mPainter.restore();
}

void RotateImagePainter::drawImage(const QPoint& pos, int angle, int srcWidth, int srcHeight, int dstWidth, int dstHeight, const QImage& image, const QRect &srcRect)
{
    qreal srcxc = pos.x() + srcWidth / 2.0;
    qreal srcyc = pos.y() + srcHeight / 2.0;

    qreal dstxc = pos.x() + dstWidth / 2.0;
    qreal dstyc = pos.y() + dstHeight / 2.0;

    QTransform t;
    t.translate(dstxc, dstyc);
    t.rotate(angle);
    t.translate(-srcxc, -srcyc);

    mPainter.setTransform(t);
    if ( srcRect == sEmptyRect )
        mPainter.drawImage(pos, image);
    else {
        QRect target(pos, QSize(dstHeight, dstWidth));
        mPainter.drawImage(target, image, srcRect);
    }
}
