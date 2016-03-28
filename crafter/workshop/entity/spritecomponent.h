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
    static const QString sComponentName;
    static ComponentId   sComponentId;

    SpriteComponent();

  // get/set
    const QSize& getSize() const {
        return mSize;
    }

    void setSize(const QSize& size) {
        mSize = size;
    }

    int getAnimationSpeed() const {
        return mAnimSpeed;
    }

    void setAnimationSpeed(int speed) {
        mAnimSpeed = speed;
    }

    void addAnimation(SpriteAnimation&& that);

  // query
    virtual ComponentId componentId() const override;

private:

    QSize       mSize;
    Animations  mAnimations;
    int         mAnimSpeed;
};

#endif // SPRITECOMPONENT_H
