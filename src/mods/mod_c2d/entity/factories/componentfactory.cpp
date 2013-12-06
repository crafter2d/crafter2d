
#include "componentfactory.h"

#include "core/defines.h"

ComponentFactory::ComponentFactory(ComponentInterface::ComponentType type):
   mpContentManager(NULL),
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
