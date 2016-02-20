#ifndef ROTATEIMAGEPAINTER_H
#define ROTATEIMAGEPAINTER_H

#include <QRect>

class QImage;
class QPainter;
class QPoint;

class RotateImagePainter
{
public:
    static const QRect sEmptyRect;

    RotateImagePainter(QPainter &painter);
    ~RotateImagePainter();

    void drawImage(const QPoint& pos, int angle, int srcWidth, int srcHeight, int dstWidth, int dstHeight, const QImage& image, const QRect& srcRect = sEmptyRect);

private:

    QPainter& mPainter;
};

#endif // ROTATEIMAGEPAINTER_H
