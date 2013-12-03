
#ifndef COMPONENT_FACTORY_H
#define COMPONENT_FACTORY_H

#include "core/entity/components/componentinterface.h"

class ContentManager;
class Component;
class ComponentDefinitionProto;

class ComponentFactory
{
public:
   explicit ComponentFactory(ComponentInterface::ComponentType type);
   virtual ~ComponentFactory() = 0;

 // get/set
   const ContentManager& getContentManager() const;
   void                  setContentManager(ContentManager& manager);

   ComponentInterface::ComponentType getComponentType() const;

 // instantiation
   virtual Component* instantiate(const ComponentDefinitionProto& definition) const = 0;

private:

 // data
   ContentManager*                   mpContentManager;
   ComponentInterface::ComponentType mComponentType;
};

#endif // COMPONENT_FACTORY_H
