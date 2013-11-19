
#ifndef MESH_COMPONENT_LOADER_H
#define MESH_COMPONENT_LOADER_H

#include "componentloader.h"

namespace Graphics
{
   class Device;
}

class MeshComponentLoader : public ComponentLoader
{
public:
   explicit MeshComponentLoader(Graphics::Device& device);

 // overrides
   virtual const String& getXmlTag() const override;
   virtual ComponentDefinition* load(const TiXmlElement& element) override;

private:

 // data
   Graphics::Device& mDevice;
};

#endif // MESH_COMPONENT_LOADER_H
