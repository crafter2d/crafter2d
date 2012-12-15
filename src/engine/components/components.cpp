
#include "components.h"

#include "component.h"

Components::Components(Entity& entity):
   mEntity(entity),
   mComponents()
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
}

// - Operations

void Components::update(float delta)
{
   ComponentMap::iterator it = mComponents.begin();
   for ( ; it != mComponents.end(); ++it )
   {
      Component* pcomponent = it->second;
      pcomponent->update(delta);
   }
}

// - Find

Component* Components::findComponent(ComponentInterface::ComponentType type)
{
   ComponentMap::iterator it = mComponents.find(type);
   return ( it != mComponents.end() ) ? it->second : NULL;
}
