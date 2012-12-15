
#include "core/defines.h"

INLINE void Component::setOwner(Components& components)
{
   mpComponents = &components;
}

INLINE ComponentInterface::ComponentType Component::getType() const
{
   return mType;
}
