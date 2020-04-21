
#include "meshcomponentfactory.h"

#include "core/content/contentmanager.h"
#include "core/entity/components/meshcomponent.h"
#include "core/graphics/device.h"
#include "core/graphics/animator.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/sprites/sprite.h"
#include "core/graphics/sprites/spritefactory.h"
#include "core/graphics/tiles/tileatlas.h"

#include "../proto/meshcomponentdefinitionproto.h"

using namespace Graphics;

MeshComponentFactory::MeshComponentFactory(Graphics::Device& device):
   ComponentFactory(ComponentInterface::eMeshComponent),
   mDevice(device),
   mSpriteDefinitions()
{
}

// Overrides

Component* MeshComponentFactory::instantiate(const ComponentDefinitionProto& definition) const
{
   const MeshComponentDefinitionProto& meshdef = static_cast<const MeshComponentDefinitionProto&>(definition);
   c2d::SpriteDefinition& spritedef = findOrAdd(meshdef);

   return new MeshComponent(c2d::SpriteFactory::create(mDevice, spritedef));
}

c2d::SpriteDefinition& MeshComponentFactory::findOrAdd(const MeshComponentDefinitionProto& definition) const
{
   auto it = mSpriteDefinitions.find(&definition);
   if ( it != mSpriteDefinitions.end() ) {
      return it->second;
   }

   c2d::SpriteDefinition spritedef;
   Size meshsize(definition.mWidth, definition.mHeight);
   spritedef.setSize(meshsize);

   if ( definition.mTexture.isEmpty() )
   {
      c2d::Animator* panimator = createAnimator(definition);
      spritedef.setSpriteAnimator(panimator);
   }
   else
   {
      auto& atlas = mDevice.getContext().getSpriteAtlas();
      spritedef.setTile(atlas.lookup(definition.mTexture));
   }

   auto pair = mSpriteDefinitions.insert(std::make_pair(&definition, std::move(spritedef)));
   return pair.first->second;
}

c2d::Animator* MeshComponentFactory::createAnimator(const MeshComponentDefinitionProto& definition) const
{
   auto& atlas = mDevice.getContext().getSpriteAtlas();

   auto presult = std::make_unique<c2d::Animator>();
   presult->setAnimationSpeed(definition.mAnimationSpeed);

   for ( const auto& animationdef : definition.mAnimations )
   {
      if ( animationdef.type == c2d::Animator::eInvalid )
      {
         // invalid will be replaced by custom where the user can specify their own animation name.
         // he should later query the id based on the name in script.
         continue;
      }

      c2d::Animator::Animation animation;
      animation.reserve(animationdef.frames.size());

      for ( auto& name : animationdef.frames )
      {
         int index = atlas.lookup(name);
         animation.push_back(index);
      }

      presult->add(animationdef.type, std::move(animation));
   }

   return presult.release();
}
