#include "spriteanimation.h"

SpriteAnimationTile::SpriteAnimationTile():
    mName()
{
}

SpriteAnimationTile::SpriteAnimationTile(const QString& tilename):
    mName(tilename)
{
}

// - Animation

SpriteAnimation::SpriteAnimation():
    mName(),
    mTiles()
{
}

SpriteAnimation::SpriteAnimation(const SpriteAnimation& that):
    mName(that.mName),
    mTiles(that.mTiles)
{
}

SpriteAnimation::SpriteAnimation(SpriteAnimation&& that):
    mName(std::move(that.mName)),
    mTiles(std::move(that.mTiles))
{
}

SpriteAnimation& SpriteAnimation::operator=(const SpriteAnimation& that)
{
    mName = that.mName;
    mTiles = that.mTiles;
    return *this;
}

SpriteAnimation& SpriteAnimation::operator=(SpriteAnimation&& that)
{
    mName = std::move(that.mName);
    mTiles = std::move(that.mTiles);
    return *this;
}

void SpriteAnimation::addTile(const QString &tilename)
{
    mTiles.append(SpriteAnimationTile(tilename));
}
