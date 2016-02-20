#ifndef SPRITE_H
#define SPRITE_H

#include <QRect>
#include <QString>

class Sprite
{
public:
    Sprite();
    Sprite(const Sprite& that);
    Sprite& operator=(const Sprite& that);

#ifdef Q_COMPILER_RVALUE_REFS
    Sprite(Sprite&& that);
    Sprite& operator=(Sprite&& that);
#endif

    void translate(const QPoint& offset) {
        mCoordinates.translate(offset);
    }

// private:

    QString mName;
    QRect mCoordinates;
    QSize mOriginalSize;
    bool mRotated;
};

#endif // SPRITE_H
