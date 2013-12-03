
#ifndef MESH_COMPONENT_FACTORY_H
#define MESH_COMPONENT_FACTORY_H

#include "componentfactory.h"

namespace Graphics
{
   class Animator;
   class Device;
}

class MeshComponentDefinitionProto;

class MeshComponentFactory : public ComponentFactory
{
public:
   MeshComponentFactory(Graphics::Device& device);

   virtual Component* instantiate(const ComponentDefinitionProto& definition) const override;

private:

 // operations
   Graphics::Animator* createAnimator(const MeshComponentDefinitionProto& definition) const;

 // data
   Graphics::Device& mDevice;

};

#endif // MESH_COMPONENT_FACTORY_H
