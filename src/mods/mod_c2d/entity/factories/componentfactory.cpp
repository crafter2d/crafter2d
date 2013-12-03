
#include "componentfactory.h"

ComponentFactory::ComponentFactory(ComponentInterface::ComponentType type):
   mComponentType(type)
{
}

ComponentFactory::~ComponentFactory()
{
}

// - Get/set

const ContentManager& ComponentFactory::getContentManager() const
{
   return *mpContentManager;
}
   
void ComponentFactory::setContentManager(ContentManager& manager)
{
   mpContentManager = &manager;
}

ComponentInterface::ComponentType ComponentFactory::getComponentType() const
{
   return mComponentType;
}
