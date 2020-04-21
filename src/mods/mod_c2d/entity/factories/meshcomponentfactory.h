
#ifndef MESH_COMPONENT_FACTORY_H
#define MESH_COMPONENT_FACTORY_H

#include "componentfactory.h"

#include <unordered_map>

#include "core/graphics/sprites/spritedefinition.h"

namespace c2d
{
   class Animator;
}

namespace Graphics
{
   class Device;
}

class MeshComponentDefinitionProto;

class MeshComponentFactory : public ComponentFactory
{
public:
   MeshComponentFactory(Graphics::Device& device);

   virtual Component* instantiate(const ComponentDefinitionProto& definition) const override;

private:
   using Map = std::unordered_map<const MeshComponentDefinitionProto*, c2d::SpriteDefinition>;

 // operations
   c2d::SpriteDefinition& findOrAdd(const MeshComponentDefinitionProto& definition) const;
   c2d::Animator* createAnimator(const MeshComponentDefinitionProto& definition) const;

 // data
   Graphics::Device& mDevice;
   mutable Map mSpriteDefinitions;
};

#endif // MESH_COMPONENT_FACTORY_H
