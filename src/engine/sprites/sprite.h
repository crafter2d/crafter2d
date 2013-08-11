
#ifndef SPRITE_H
#define SPRITE_H

#include "core/math/size.h"
#include "core/math/xform.h"
#include "core/resource/resourcemanager.h"

#include "engine/texturecoordinate.h"

namespace Graphics
{
   class Texture;
}

class Sprite
{
public:
   Sprite();

 // get/set
   const Size&              getSize() const;
   void                     setSize(const Size& size);

   const Size&              getHalfSize() const;

   const Graphics::Texture& getTexture() const;
   void                     setTexture(const TexturePtr& texture);

   const TextureCoordinate& getTextureCoordinate() const;
   void                     setTextureCoordinate(const TextureCoordinate& coordinate);

   const XForm&             getTransform() const;
   void                     setTransform(const XForm& xform);

private:

 // data
   TexturePtr        mTexture;
   TextureCoordinate mTexCoordinate;
   XForm             mTransform;
   Size              mSize;
   Size              mHalfSize;
};

#endif // SPRITE_H
