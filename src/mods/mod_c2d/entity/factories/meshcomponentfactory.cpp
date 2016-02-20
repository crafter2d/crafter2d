
#include "meshcomponentfactory.h"

#include "core/content/contentmanager.h"
#include "core/entity/components/meshcomponent.h"
#include "core/graphics/device.h"
#include "core/graphics/animator.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/sprites/sprite.h"
#include "core/graphics/sprites/spritedefinition.h"
#include "core/graphics/sprites/spritefactory.h"
#include "core/graphics/tiles/tileatlas.h"

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
   c2d::Animator* panimator = createAnimator(meshdef);

   auto pspritedef = new c2d::SpriteDefinition();
   pspritedef->setSize(meshsize);
   pspritedef->setSpriteAnimator(panimator);

   MeshComponent* presult = new MeshComponent();
   presult->setSprite(c2d::SpriteFactory::create(mDevice, pspritedef));
   return presult;
}

c2d::Animator* MeshComponentFactory::createAnimator(const MeshComponentDefinitionProto& definition) const
{
   auto& atlas = mDevice.getContext().getSpriteAtlas();

   c2d::Animator* presult = new c2d::Animator();
   presult->setAnimationSpeed(definition.mAnimationSpeed);

   for ( uint32_t index = 0; index < definition.mAnimations.size(); ++index )
   {
      auto& animationdef = definition.mAnimations[index];

      c2d::Animator::Animation& animation = presult->emplaceAnimation();
      animation.reserve(animationdef.frames.size());

      for ( auto& name : animationdef.frames )
      {
         int index = atlas.lookup(name);
         animation.push_back(index);
      }
   }

   return presult;
}
