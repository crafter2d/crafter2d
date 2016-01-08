#include "spriteanimation.h"

SpriteAnimation::SpriteAnimation():
    mName(),
    mFrameCount(0)
{
}

SpriteAnimation::SpriteAnimation(const SpriteAnimation& that):
    mName(that.mName),
    mFrameCount(that.mFrameCount)
{
}

SpriteAnimation::SpriteAnimation(SpriteAnimation&& that):
    mName(std::move(that.mName)),
    mFrameCount(that.mFrameCount)
{
    that.mFrameCount = 0;
}

SpriteAnimation& SpriteAnimation::operator=(const SpriteAnimation& that)
{
    mName = that.mName;
    mFrameCount = that.mFrameCount;
    return *this;
}

SpriteAnimation& SpriteAnimation::operator=(SpriteAnimation&& that)
{
    mName = std::move(that.mName);
    mFrameCount = that.mFrameCount;

    that.mFrameCount = 0;

    return *this;
}
