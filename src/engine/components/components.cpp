
#include "components.h"

#include "component.h"
#include "componentmessage.h"

Components::Components(Entity& entity):
   mEntity(entity),
   mComponents(),
   mMessageToComponent()
{
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
