
#include "physicscomponent.h"
#ifndef JENGINE_INLINE
#  include "physicscomponent.inl"
#endif

#include "engine/physics/body.h"

#include "componentmessage.h"
#include "componentstructs.h"

PhysicsComponent::PhysicsComponent():
   Component(ComponentInterface::ePhysisComponent),
   mpBody(NULL),
   mTransform()
{
}

// - Component interface

void PhysicsComponent::handleMessage(const ComponentMessage& message)
{
}

void PhysicsComponent::update(float delta)
{
   const Matrix4& transform = mpBody->getTransform();

   if ( transform != mTransform )
   {
      mTransform = transform;
      
      ComponentMessage message(ComponentInterface::ePositionChangedMsg, &mTransform);
      postMessage(message);
   }
}
