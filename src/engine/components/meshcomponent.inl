
#include "core/defines.h"

INLINE const Animator& MeshComponent::getAnimator() const
{
   return *mpAnimator;
}
   
INLINE void MeshComponent::setAnimator(Animator* panimator)
{
   mpAnimator = panimator;
}

INLINE const String& MeshComponent::getEffectName() const
{
   return mEffectName;
}
   
INLINE void MeshComponent::setEffectName(const String& name)
{
   mEffectName = name;
}

INLINE const Size& MeshComponent::getSize() const
{
   return mSize;
}
   
INLINE void MeshComponent::setSize(const Size& size)
{
   mSize = size;
}
