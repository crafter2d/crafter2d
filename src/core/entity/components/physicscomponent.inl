
#include "core/defines.h" 

// - Get/set

INLINE bool PhysicsComponent::hasBody() const
{
   return mpBody != nullptr;
}

INLINE const Body& PhysicsComponent::getBody() const
{
   ASSERT_PTR(mpBody);
   return *mpBody;
}

INLINE Body& PhysicsComponent::getBody()
{
   ASSERT_PTR(mpBody);
   return *mpBody;
}
