
#include "sprite.h"

#include "core/graphics/texture.h"

using namespace Graphics;

Sprite::Sprite():
   mTexture(),
   mTexCoordinate(),
   mTransform(),
   mSize()
{
}

// - Query

const Size& Sprite::getSize() const
{
   return mSize;
}

void Sprite::setSize(const Size& size)
{
   mSize = size;
}

const Texture& Sprite::getTexture() const
{
   return *mTexture;
}

const TextureCoordinate& Sprite::getTextureCoordinate() const
{
   return mTexCoordinate;
}

void Sprite::setTextureCoordinate(const TextureCoordinate& coordinate)
{
   mTexCoordinate = coordinate;
}

const XForm& Sprite::getTransform() const
{
   return mTransform;
}

void Sprite::setTransform(const XForm& xform)
{
}
