
#ifndef ENTITY_COMPONENT_H
#define ENTITY_COMPONENT_H

#include "core/core_base.h"

#include "componentinterface.h"

class Components;
class ComponentMessage;
class Entity;

class CORE_API Component
{
public:
   explicit Component(ComponentInterface::ComponentType type);
   virtual ~Component();

 // get/set
   Entity& getEntity();

   ComponentInterface::ComponentType getType() const {
      return mType;
   }

 // operations
   virtual void registerComponent(Components& components);
   virtual void handleMessage(ComponentMessage& message) = 0;

   //virtual void update(float delta);

protected:
 // messaging
	void postMessage(ComponentMessage& message);

private:

   Components*                       mpComponents;
   ComponentInterface::ComponentType mType;
};

#endif // ENTITY_COMPONENT_H
