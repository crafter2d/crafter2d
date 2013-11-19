
#ifndef COMPONENT_FACTORIES_H
#define COMPONENT_FACTORIES_H

#include <vector>

class Component;
class ComponentDefinitionProto;
class ComponentFactory;

class ComponentFactories : public std::vector<ComponentFactory*>
{
public:

 // instantiation
   Component* instantiate(ComponentDefinitionProto& definition);
}

#endif // COMPONENT_FACTORIES_H
