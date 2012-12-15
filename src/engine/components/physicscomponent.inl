
#include "core/defines.h" 

// - Get/set

INLINE bool PhysicsComponent::hasBody() const
{
   return mpBody != NULL;
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

INLINE void PhysicsComponent::setBody(Body& body)
{
   mpBody = &body;
}

