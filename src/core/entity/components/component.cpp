
#include "component.h"

#include "core/defines.h"

#include "components.h"

using namespace ComponentInterface;

Component::Component(ComponentType type):
   mpComponents(nullptr),
   mType(type)
{
}

Component::~Component()
{
}

// - Get/set

Entity& Component::getEntity()
{
   return mpComponents->getEntity();
}

// - Messaging

void Component::postMessage(ComponentMessage& message)
{
	ASSERT_PTR(mpComponents);
	mpComponents->postMessage(message);
}

// - Operations

void Component::registerComponent(Components& components)
{
	mpComponents = &components;
}
