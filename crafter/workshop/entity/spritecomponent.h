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

    const Animations& getAnimations() const {
        return mAnimations;
    }

    const QString& getTexture() const {
        return mTexture;
    }

    void setTexture(const QString& texture) {
        mTexture = texture;
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
    QString     mTexture;
    Animations  mAnimations;
    int         mAnimSpeed;
};

#endif // SPRITECOMPONENT_H
