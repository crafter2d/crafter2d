
#include "component.h"
#ifndef JENGINE_INLINE
#  include "component.inl"
#endif

#include "components.h"

using namespace ComponentInterface;

Component::Component(ComponentType type):
   mpComponents(NULL),
   mType(type)
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
