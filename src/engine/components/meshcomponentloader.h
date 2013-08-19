
#ifndef MESH_COMPONENT_LOADER_H
#define MESH_COMPONENT_LOADER_H

#include "componentloader.h"

class MeshComponentLoader : public ComponentLoader
{
public:
   MeshComponentLoader();

 // overrides
   virtual const String& getXmlTag() const override;
   virtual ComponentDefinition* load(const TiXmlElement& element) override;
};

#endif // MESH_COMPONENT_LOADER_H
