
#ifndef SPRITE_DEFINITION_H
#define SPRITE_DEFINITION_H

#include "core/resource/resourcemanager.h"

#include "engine/animator.h"

class SpriteDefinition
{
public:

 // get/set
   int getWidth() const;
   void setWidth(int width);

   int getHeight() const;
   void setHeight(int height);


private:

 // data
   TexturePtr  mTexture;
   AnimatorPtr mAnimator;
   int         mWidth;
   int         mHeight;

};

#endif // SPRITE_DEFINITION_H
