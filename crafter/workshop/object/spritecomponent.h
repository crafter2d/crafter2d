#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <QSize>
#include <QString>
#include <QVector>

#include "entitycomponent.h"

#include "spriteanimation.h"

class SpriteComponent : public EntityComponent
{
    using Animations = QVector<SpriteAnimation>;

public:
    SpriteComponent();

    const QSize& getSize() const {
        return mSize;
    }

    void setSize(const QSize& size) {
        mSize = size;
    }

    const QString& getTextureName() const {
        return mTexName;
    }

    void setTextureName(const QString& filename) {
        mTexName = filename;
    }

    int getAnimationSpeed() const {
        return mSpeed;
    }

    void setAnimationSpeed(int speed) {
        mSpeed = speed;
    }

    void addAnimation(SpriteAnimation&& that);

private:

    QSize mSize;
    QString mTexName;
    int mSpeed;
    Animations  mAnimations;
};

#endif // SPRITECOMPONENT_H
