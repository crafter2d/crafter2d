
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

// - Get/set

void PhysicsComponent::setBody(Body& body)
{
   mpBody = &body;
   mpBody->addListener(this);
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
   PositionChangedInfo info;
   info.position = body.getPosition();
   info.angle = body.getAngle();

   ComponentMessage message(ComponentInterface::ePositionChangedMsg, &info);
   postMessage(message);
}
