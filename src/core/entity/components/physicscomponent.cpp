
#include "physicscomponent.h"
#ifndef JENGINE_INLINE
#  include "physicscomponent.inl"
#endif

#include "core/physics/body.h"
#include "core/entity/entity.h"
#include "core/entity/componentmessages/querybodycomponentmessage.h"

#include "components.h"
#include "componentstructs.h"

PhysicsComponent::PhysicsComponent():
   Component(ComponentInterface::ePhysisComponent),
   mpBody(nullptr)
{
}

// - Get/set

void PhysicsComponent::setBody(Body& body)
{
   mpBody = &body;
   mpBody->addListener(this);
}

// - Component interface

void PhysicsComponent::registerComponent(Components& components)
{
	Component::registerComponent(components);

	components.subscribeMessageType(*this, ComponentInterface::ePositionChangedMsg);
   components.subscribeMessageType(*this, ComponentInterface::eRotationChangedMsg);
   components.subscribeMessageType(*this, ComponentInterface::eQueryBodyMsg);

   mpBody->setEntity(getEntity());
}

void PhysicsComponent::handleMessage(ComponentMessage& message)
{
   using namespace ComponentInterface;

   switch ( message.getMessageType() )
   {
      case ePositionChangedMsg:
      case eRotationChangedMsg:
         {
            mpBody->setTransform(getEntity().getTransform());
         }
         break;
      case eQueryBodyMsg:
         {
            QueryBodyComponentMessage& query = static_cast<QueryBodyComponentMessage&>(message);
            query.setBody(*mpBody);
         }
         break;
      default:
         break;
   }
}

// - Notifications

void PhysicsComponent::onPositionChanged(Body& body)
{
   getEntity().setTransform(body.getTransform());
}

