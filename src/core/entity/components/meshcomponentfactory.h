
#ifndef MESH_COMPONENT_FACTORY_H
#define MESH_COMPONENT_FACTORY_H

#include "componentfactory.h"

namespace Graphics
{
   class Device;
}

class MeshComponentFactory : public ComponentFactory
{
public:
   MeshComponentFactory(Graphics::Device& device);

   virtual ComponentLoader* createLoader() const override;

   virtual Component* instantiate(const ComponentDefinition& definition) const override;

private:

 // data
   Graphics::Device& mDevice;

};

#endif // MESH_COMPONENT_FACTORY_H
