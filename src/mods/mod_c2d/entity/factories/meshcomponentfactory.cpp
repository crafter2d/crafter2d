
#include "meshcomponentfactory.h"

#include "core/entity/components/meshcomponent.h"
#include "core/smartptr/autoptr.h"
#include "core/graphics/sprites/sprite.h"
#include "core/graphics/sprites/spritefactory.h"

#include "../proto/meshcomponentdefinitionproto.h"

MeshComponentFactory::MeshComponentFactory(Graphics::Device& device):
   ComponentFactory(ComponentInterface::eMeshComponent),
   mDevice(device)
{
}

// Overrides

Component* MeshComponentFactory::instantiate(const ComponentDefinitionProto& definition) const
{
   const MeshComponentDefinitionProto& meshdef = static_cast<const MeshComponentDefinitionProto&>(definition);

   AutoPtr<Graphics::Sprite> sprite = Graphics::SpriteFactory::create(mDevice, meshdef.getSpriteDefinition());

   MeshComponent* presult = new MeshComponent();
   presult->setSprite(sprite.release());
   return presult;
}
