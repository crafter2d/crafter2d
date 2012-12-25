
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
}

// - Notifications

void PhysicsComponent::onPositionChanged(Body& body)
{
   mTransform.setPosition(body.getPosition());
   mTransform.setRotation(body.getAngle());

   ComponentMessage message(ComponentInterface::ePositionChangedMsg, &mTransform);
   postMessage(message);
}
