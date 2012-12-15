
#ifndef ENTITY_COMPONENTS_H
#define ENTITY_COMPONENTS_H

#include <map>

#include "componentinterface.h"

class Component;
class Entity;

class Components
{
public:
   Components(Entity& entity);

 // get/set
   Entity& getEntity();

 // maintenance
   void addComponent(Component* pcomponent);

 // operations
   void update(float delta);

 // find
   Component* findComponent(ComponentInterface::ComponentType type);

private:
 // typedefs
   typedef std::map<ComponentInterface::ComponentType, Component*> ComponentMap;

 // members
   Entity&        mEntity;
   ComponentMap   mComponents;
};

#endif // ENTITY_COMPONENTS_H
