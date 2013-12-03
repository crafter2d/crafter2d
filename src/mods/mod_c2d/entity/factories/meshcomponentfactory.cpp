
#include "meshcomponentfactory.h"

#include "core/content/contentmanager.h"
#include "core/entity/components/meshcomponent.h"
#include "core/graphics/device.h"
#include "core/graphics/animator.h"
#include "core/graphics/sprites/sprite.h"
#include "core/graphics/sprites/spritedefinition.h"
#include "core/graphics/sprites/spritefactory.h"

#include "../proto/meshcomponentdefinitionproto.h"

using namespace Graphics;

MeshComponentFactory::MeshComponentFactory(Graphics::Device& device):
   ComponentFactory(ComponentInterface::eMeshComponent),
   mDevice(device)
{
}

// Overrides

Component* MeshComponentFactory::instantiate(const ComponentDefinitionProto& definition) const
{
   const MeshComponentDefinitionProto& meshdef = static_cast<const MeshComponentDefinitionProto&>(definition);

   Size meshsize(meshdef.mWidth, meshdef.mHeight);
   Animator* panimator = createAnimator(meshdef);
   Texture* ptexture = getContentManager().loadContent<Texture>(meshdef.mTexture);
   panimator->initialize(*ptexture, meshsize);

   SpriteDefinition spritedef;
   spritedef.setSize(meshsize);
   spritedef.setTexture(ptexture);
   spritedef.setSpriteAnimator(panimator);

   MeshComponent* presult = new MeshComponent();
   presult->setSprite(SpriteFactory::create(mDevice, spritedef));
   return presult;
}

Animator* MeshComponentFactory::createAnimator(const MeshComponentDefinitionProto& definition) const
{
   Animator* presult = new Animator();
   presult->setAnimationSpeed(definition.mAnimationSpeed);

   int start = 0;
   for ( uint32_t index = 0; index < definition.mAnimations.size(); ++index )
   {
      int length = definition.mAnimations[index];

      presult->addAnimation(start, length);

      start += length;
   }

   return presult;
}
