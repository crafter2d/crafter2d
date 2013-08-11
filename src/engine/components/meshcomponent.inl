
#include "core/defines.h"

INLINE const Animator& MeshComponent::getAnimator() const
{
   return *mpAnimator;
}
   
INLINE void MeshComponent::setAnimator(Animator* panimator)
{
   mpAnimator = panimator;
}

INLINE const Sprite& MeshComponent::getSprite() const
{
   ASSERT_PTR(mpSprite);
   return *mpSprite;
}
   
INLINE void MeshComponent::setSprite(Sprite* psprite)
{
   mpSprite = psprite;
}

