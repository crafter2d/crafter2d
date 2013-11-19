
#include "meshcomponentfactory.h"

#include "core/smartptr/autoptr.h"

#include "core/graphics/sprites/sprite.h"
#include "core/graphics/sprites/spritefactory.h"

#include "meshcomponent.h"
#include "meshcomponentdefinition.h"
#include "meshcomponentloader.h"

MeshComponentFactory::MeshComponentFactory(Graphics::Device& device):
   ComponentFactory(ComponentInterface::eMeshComponent),
   mDevice(device)
{
}

ComponentLoader* MeshComponentFactory::createLoader() const
{
   return new MeshComponentLoader(mDevice);
}

Component* MeshComponentFactory::instantiate(const ComponentDefinition& definition) const
{
   const MeshComponentDefinition& meshdef = static_cast<const MeshComponentDefinition&>(definition);

   AutoPtr<Graphics::Sprite> sprite = Graphics::SpriteFactory::create(mDevice, meshdef.getSpriteDefinition());

   MeshComponent* presult = new MeshComponent();
   presult->setSprite(sprite.release());
   return presult;
}
