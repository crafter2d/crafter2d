
#ifndef COMPONENT_DEFINITION_H
#define COMPONENT_DEFINITION_H

#include "componentinterface.h"

class Component;

class ComponentDefinition
{
public:
   explicit ComponentDefinition(ComponentInterface::ComponentType type);
   virtual ~ComponentDefinition() = 0;

 // get/set
   ComponentInterface::ComponentType getComponentType() const;

private:

 // data
   ComponentInterface::ComponentType mComponentType;
};

#endif // COMPONENT_DEFINITION_H
