
#pragma once

#include <map>
#include <set>

#include "core/core_base.h"

#include "componentinterface.h"

class Component;
class ComponentMessage;
class Entity;

class CORE_API Components
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
   using ComponentMap = std::map<ComponentInterface::ComponentType, Component*>;
   using MessageToComponentSet = std::set<ComponentInterface::ComponentType>;

 // members
   Entity&					   mEntity;
   ComponentMap				mComponents;
   MessageToComponentSet	mMessageToComponent[ComponentInterface::eNUM_MESSAGE_TYPES];
};
