
#include "netcomponent.h"

#include "core/entity/entity.h"
#include "core/entity/componentmessages/netstreamcomponentmessage.h"
#include "core/streams/datastream.h"
#include "core/defines.h"

#include "components.h"

NetComponent::NetComponent():
   Component(ComponentInterface::eNetComponent),
   mDirtyFlag(0)
{
}

// - Overrides

void NetComponent::registerComponent(Components& components)
{
   Component::registerComponent(components);

   components.subscribeMessageType(*this, ComponentInterface::ePositionChangedMsg);
   components.subscribeMessageType(*this, ComponentInterface::eRotationChangedMsg);
	components.subscribeMessageType(*this, ComponentInterface::eNetStreamMsg);
}
   
void NetComponent::handleMessage(ComponentMessage& message)
{
   using namespace ComponentInterface;

   switch ( message.getMessageType() )
   {
   case ePositionChangedMsg:
      SET_FLAG(mDirtyFlag, ePositionDirty);
      break;
   case eRotationChangedMsg:
      SET_FLAG(mDirtyFlag, eRotationChangedMsg);
      break;
   case eNetStreamMsg:
      {
         NetStreamComponentMessage& netmessage = static_cast<NetStreamComponentMessage&>(message);
         handleStreamMessage(netmessage);
      }
      break;
   }
}

// - Message handling

void NetComponent::handleStreamMessage(NetStreamComponentMessage& message)
{
   DataStream& stream = message.getStream();

   if ( message.isWrite() )
   {
      if ( mDirtyFlag != 0 )
      {
         writeToStream(stream);
      }
   }
   else
   {
      readFromStream(stream);
   }

   mDirtyFlag = 0;
}

void NetComponent::writeToStream(DataStream& stream)
{
   Entity& entity = getEntity();

   stream.writeUint(mDirtyFlag);

   if ( IS_SET(mDirtyFlag, Entity::ePositionDirty) )
   {
      stream << entity.getTransform().getPosition().x << entity.getTransform().getPosition().y;
   }

   if ( IS_SET(mDirtyFlag, Entity::eRotationDirty) )
   {
      stream << entity.getTransform().getAngle();
   }

   if ( IS_SET(mDirtyFlag, eNameDirty) )
   {
      stream << entity.getName();
   }
}

void NetComponent::readFromStream(DataStream& stream)
{
   Entity& entity = getEntity();

   stream.readUint(mDirtyFlag);
   ASSERT(mDirtyFlag > 0);

   if ( IS_SET(mDirtyFlag, Entity::ePositionDirty) )
   {
      Vector pos;
      stream >> pos.x >> pos.y;
      entity.setPosition(pos);
   }

   if ( IS_SET(mDirtyFlag, Entity::eRotationDirty) )
   {
      float angle;
      stream >> angle;
      entity.setRotation(angle);
   }

   if ( IS_SET(mDirtyFlag, eNameDirty) )
   {
      String name;
      stream >> name;
      entity.setName(name);
   }
}

