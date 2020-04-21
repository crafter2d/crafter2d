
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
   explicit Components(Entity& entity);
           ~Components();

 // get/set
   Entity& getEntity();

 // maintenance
   void addComponent(Component* pcomponent);
   void clear();

 // messaging
   void subscribeMessageType(Component& component, ComponentInterface::ComponentMessageType messagetype);
   void postMessage(ComponentMessage& message);

 // find
   template<typename Type>
   Type* get(ComponentInterface::ComponentType type) {
      return static_cast<Type*>(findComponent(type));
   }
   Component* findComponent(ComponentInterface::ComponentType type);

private:
 // typedefs
   typedef std::map<ComponentInterface::ComponentType, Component*> ComponentMap;
   typedef std::set<ComponentInterface::ComponentType> MessageToComponentSet;

 // members
   Entity&					   mEntity;
   ComponentMap				mComponents;
   MessageToComponentSet	mMessageToComponent[ComponentInterface::eNUM_MESSAGE_TYPES];
};

#endif // ENTITY_COMPONENTS_H
