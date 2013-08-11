
#ifndef COMPONENT_DEFINITION_H
#define COMPONENT_DEFINITION_H

class Component;
class TiXmlElement;

class ComponentDefinition
{
public:

   ComponentDefinition();
   virtual ~ComponentDefinition() = 0;

   virtual void load(const TiXmlElement& element) = 0;

   virtual Component* instantiate() const = 0;
};

#endif // COMPONENT_DEFINITION_H
