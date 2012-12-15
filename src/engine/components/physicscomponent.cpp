
#include "physicscomponent.h"
#ifndef JENGINE_INLINE
#  include "physicscomponent.inl"
#endif

#include "engine/physics/body.h"

PhysicsComponent::PhysicsComponent():
   Component(ComponentInterface::ePhysisComponent),
   mpBody(NULL),
   mPosition(),
   mAngle(0.0f)
{
}

// - Component interface

void PhysicsComponent::handleMessage(const ComponentMessage& message)
{
}

void PhysicsComponent::update(float delta)
{
   if ( mPosition != mpBody->getPosition() )
   {
      // moved
      mPosition = mpBody->getPosition();
   }

   if ( mAngle != mpBody->getAngle() )
   {
      // rotated
      mAngle = mpBody->getAngle();
   }
}
