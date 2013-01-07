
#ifndef ENTITY_COMPONENTS_H
#define ENTITY_COMPONENTS_H

#include <map>
#include <set>

#include "componentinterface.h"

class Component;
class ComponentMessage;
class Entity;

class Components
{
public:
   Components(Entity& entity);

 // get/set
   Entity& getEntity();

 // maintenance
   void addComponent(Component* pcomponent);

 // messaging
   void subscribeMessageType(Component& component, ComponentInterface::ComponentMessageType messagetype);
   void postMessage(ComponentMessage& message);

 // find
   Component* findComponent(ComponentInterface::ComponentType type);

private:
 // typedefs
   typedef std::map<ComponentInterface::ComponentType, Component*> ComponentMap;
   typedef std::set<ComponentInterface::ComponentType> MessageToComponentSet;

 // members
   Entity&					mEntity;
   ComponentMap				mComponents;
   MessageToComponentSet	mMessageToComponent[ComponentInterface::eNUM_MESSAGE_TYPES];
};

#endif // ENTITY_COMPONENTS_H
