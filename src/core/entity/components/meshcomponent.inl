
#include "core/defines.h"

INLINE const Graphics::Sprite& MeshComponent::getSprite() const
{
   ASSERT_PTR(mpSprite);
   return *mpSprite;
}
   
INLINE void MeshComponent::setSprite(Graphics::Sprite* psprite)
{
   mpSprite = psprite;
}

