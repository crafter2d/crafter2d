
#include <map>
#include <memory>

#include "meshcomponentloader.h"

#include "core/string/string.h"
#include "core/graphics/animator.h"

#include "proto/entitydefinitionproto.h"
#include "proto/meshcomponentdefinitionproto.h"

static String sId(UTEXT("sprite"));

std::map<std::string, c2d::Animator::AnimationType> AnimationMap = {
   { "idle", c2d::Animator::eIdle },
   { "walk", c2d::Animator::eWalking },
   { "run", c2d::Animator::eRunning },
   { "jump", c2d::Animator::eJumping },
   { "slide", c2d::Animator::eSliding },
   { "die", c2d::Animator::eDying },
};

namespace c2d
{

   // - statics

   void MeshComponentLoader::load(EntityDefinitionProto& entity, const std::vector<entity_definitions::sprite>& sprites)
   {
      MeshComponentLoader loader;
      for ( auto& sprite : sprites )
      {
         auto pcomponent = loader.load(sprite);
         entity.mComponents.push_back(pcomponent);
      }
   }

   // - constructs

   MeshComponentLoader::MeshComponentLoader()
   {
   }

   ComponentDefinitionProto* MeshComponentLoader::load(const entity_definitions::sprite& sprite)
   {
      auto pdefinition = std::make_unique<MeshComponentDefinitionProto>();
      pdefinition->mWidth = sprite.width;
      pdefinition->mHeight = sprite.height;

      if ( sprite.animationss.size() > 0 )
      {
         auto& animations = sprite.animationss[0];

         // query the animation speed (in mm)
         pdefinition->mAnimationSpeed = animations.speed / 1000.0f;

         for ( auto& anim : animations.anims )
         {
            MeshComponentDefinitionProto::Animation animation;

            auto it = AnimationMap.find(anim.name);
            animation.type = it != AnimationMap.end() ? it->second : c2d::Animator::eInvalid;

            for ( auto& tile : anim.tiles )
            {
               animation.frames.push_back(String(tile.name));
            }

            pdefinition->mAnimations.push_back(std::move(animation));
         }
      }

      if ( sprite.textures.size() == 1 )
      {
         auto& texture = sprite.textures[0];
         pdefinition->mTexture = texture.name;
      }

      return pdefinition.release();
   }
}
