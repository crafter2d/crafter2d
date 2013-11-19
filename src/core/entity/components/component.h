
#ifndef ENTITY_COMPONENT_H
#define ENTITY_COMPONENT_H

#include "componentinterface.h"

class Components;
class ComponentMessage;
class Entity;

class Component
{
public:
   Component(ComponentInterface::ComponentType type);

 // get/set
   const Entity& getEntity() const;
         Entity& getEntity();

   ComponentInterface::ComponentType getType() const;

 // operations
   virtual void registerComponent(Components& components);
   virtual void handleMessage(ComponentMessage& message) = 0;
   virtual void update(float delta) = 0;

protected:
 // messaging
	void postMessage(ComponentMessage& message);

private:

   Components*                       mpComponents;
   ComponentInterface::ComponentType mType;
};

#ifdef JENGINE_INLINE
#  include "entitycomponent.inl"
#endif

#endif // ENTITY_COMPONENT_H
