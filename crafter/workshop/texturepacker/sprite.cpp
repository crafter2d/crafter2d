#include "sprite.h"

Sprite::Sprite():
    mName(),
    mCoordinates(),
    mOriginalSize(),
    mRotated(false)
{

}

Sprite::Sprite(const Sprite& that):
    mName(that.mName),
    mCoordinates(that.mCoordinates),
    mOriginalSize(that.mOriginalSize),
    mRotated(that.mRotated)
{
}

Sprite& Sprite::operator=(const Sprite& that)
{
    mName = that.mName;
    mCoordinates = that.mCoordinates;
    mOriginalSize = that.mOriginalSize;
    mRotated = that.mRotated;
    return *this;
}

#ifdef Q_COMPILER_RVALUE_REFS

Sprite::Sprite(Sprite&& that):
    mName(std::move(that.mName)),
    mCoordinates(that.mCoordinates),
    mOriginalSize(that.mOriginalSize),
    mRotated(that.mRotated)
{
}

Sprite& Sprite::operator=(Sprite&& that)
{
    mName = std::move(that.mName);
    mCoordinates = that.mCoordinates;
    mOriginalSize = that.mOriginalSize;
    mRotated = that.mRotated;
    return *this;
}

#endif
