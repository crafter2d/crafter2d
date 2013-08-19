
#ifndef COMPONENT_FACTORY_H
#define COMPONENT_FACTORY_H

#include "componentinterface.h"

class Component;
class ComponentDefinition;
class ComponentLoader;

class ComponentFactory
{
public:
   explicit ComponentFactory(ComponentInterface::ComponentType type);
   virtual ~ComponentFactory() = 0;

   ComponentInterface::ComponentType getComponentType() const;

   virtual ComponentLoader* createLoader() const = 0;
   virtual Component* instantiate(const ComponentDefinition& definition) const = 0;

private:

 // data
   ComponentInterface::ComponentType mComponentType;
};

#endif // COMPONENT_FACTORY_H
