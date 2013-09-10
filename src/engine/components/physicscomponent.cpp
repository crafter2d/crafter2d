
#include "physicscomponent.h"
#ifndef JENGINE_INLINE
#  include "physicscomponent.inl"
#endif

#include "engine/physics/body.h"

#include "components.h"
#include "componentmessage.h"
#include "componentstructs.h"
#include "querybodycomponentmessage.h"

PhysicsComponent::PhysicsComponent():
   Component(ComponentInterface::ePhysisComponent),
   mpBody(NULL)
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

   components.subscribeMessageType(*this, ComponentInterface::ePositionMsg);
   components.subscribeMessageType(*this, ComponentInterface::eQueryPositionMsg);
   components.subscribeMessageType(*this, ComponentInterface::eQueryBodyMsg);
   
   mpBody->setEntity(getEntity());
}

void PhysicsComponent::handleMessage(ComponentMessage& message)
{
   using namespace ComponentInterface;

   switch ( message.getMessageType() )
   {
      case ePositionMsg:
         {
            PositionInfo* pinfo = reinterpret_cast<PositionInfo*>(message.getData());
            mpBody->setTransform(pinfo->transform);
         }
         break;
      case eQueryPositionMsg:
         {
            PositionInfo* pinfo = reinterpret_cast<PositionInfo*>(message.getData());
            pinfo->transform = mpBody->getTransform();
         }
         break;
      case eQueryBodyMsg:
         {
            QueryBodyComponentMessage& query = static_cast<QueryBodyComponentMessage&>(message);
            query.setBody(*mpBody);
         }
         break;
   }
}

void PhysicsComponent::update(float delta)
{
}

// - Notifications

void PhysicsComponent::onPositionChanged(Body& body)
{
   PositionInfo info;
   info.transform = body.getTransform();

   ComponentMessage message(ComponentInterface::ePositionChangedMsg, &info);
   postMessage(message);
}
