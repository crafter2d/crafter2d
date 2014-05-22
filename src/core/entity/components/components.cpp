
#include "components.h"

#include "core/entity/componentmessages/componentmessage.h"

#include "component.h"

Components::Components(Entity& entity):
   mEntity(entity),
   mComponents(),
   mMessageToComponent()
{
}

Components::~Components()
{
   clear();
}

// - Get/set

Entity& Components::getEntity()
{
   return mEntity;
}

// - Maintenance

void Components::addComponent(Component* pcomponent)
{
   mComponents[pcomponent->getType()] = pcomponent;
   pcomponent->registerComponent(*this);
}

void Components::clear()
{
   ComponentMap::iterator it = mComponents.begin();
   for ( ; it != mComponents.end(); ++it )
   {
      Component* pcomponent = it->second;
      delete pcomponent;
   }
   mComponents.clear();
   
   for ( int index = 0; index < ComponentInterface::eNUM_MESSAGE_TYPES; ++index )
   {
      mMessageToComponent[index].clear();
   }
}

// - Messaging

void Components::subscribeMessageType(Component& component, ComponentInterface::ComponentMessageType messagetype)
{
   mMessageToComponent[messagetype].insert(component.getType());
}

void Components::postMessage(ComponentMessage& message)
{
   MessageToComponentSet& set = mMessageToComponent[message.getMessageType()];
   for ( MessageToComponentSet::iterator it = set.begin(); it != set.end(); ++it )
   {
      Component* pcomponent = mComponents[*it];
      pcomponent->handleMessage(message);
   }
}

// - Find

Component* Components::findComponent(ComponentInterface::ComponentType type)
{
   ComponentMap::iterator it = mComponents.find(type);
   return ( it != mComponents.end() ) ? it->second : NULL;
}
