
#ifndef SPRITE_DEFINITION_H
#define SPRITE_DEFINITION_H

#include "core/resource/resourcemanager.h"
#include "core/string/string.h"
#include "core/math/size.h"

#include "engine/animator.h"

class SpriteDefinition
{
public:
   SpriteDefinition();

 // get/set
   const Size& getSize() const;
   void        setSize(const Size& size);

   const String& getTextureName() const;
   void          setTextureName(const String& name);

   bool            hasSpriteAnimator() const;
   const Animator& getSpriteAnimator() const;
   void            setSpriteAnimator(Animator* panimator);

private:

 // data
   String      mTextureName;
   TexturePtr  mTexture;
   Size        mSize;
   Animator*   mpAnimator;
};

#endif // SPRITE_DEFINITION_H
