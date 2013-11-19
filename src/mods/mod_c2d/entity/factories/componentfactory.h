
#ifndef COMPONENT_FACTORY_H
#define COMPONENT_FACTORY_H

#include "core/entity/components/componentinterface.h"

class Component;
class ComponentDefinitionProto;

class ComponentFactory
{
public:
   explicit ComponentFactory(ComponentInterface::ComponentType type);
   virtual ~ComponentFactory() = 0;

   ComponentInterface::ComponentType getComponentType() const;

   virtual Component* instantiate(const ComponentDefinitionProto& definition) const = 0;

private:

 // data
   ComponentInterface::ComponentType mComponentType;
};

#endif // COMPONENT_FACTORY_H
