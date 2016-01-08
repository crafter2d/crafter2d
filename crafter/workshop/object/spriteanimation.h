#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include <QString>


class SpriteAnimation
{
public:
    SpriteAnimation();
    SpriteAnimation(const SpriteAnimation& that);
    SpriteAnimation(SpriteAnimation&& that);

    SpriteAnimation& operator=(const SpriteAnimation& that);
    SpriteAnimation& operator=(SpriteAnimation&& that);

    const QString& getName() const {
        return mName;
    }

    void setName(const QString& name) {
        mName = name;
    }

    int getFrameCount() const {
        return mFrameCount;
    }

    void setFrameCount(int count) {
        mFrameCount = count;
    }

private:

    QString mName;
    int     mFrameCount;
};

#endif // SPRITEANIMATION_H
