
#ifndef PHYSICS_COMPONENT_LOADER_H
#define PHYSICS_COMPONENT_LOADER_H

#include "componentloader.h"

class PhysicsComponentLoader : public ComponentLoader
{
public:
   // overrides
   virtual const String& getXmlTag() const override;
   virtual ComponentDefinitionProto* load(const TiXmlElement& element) override;
};

#endif // PHYSICS_COMPONENT_LOADER_H
