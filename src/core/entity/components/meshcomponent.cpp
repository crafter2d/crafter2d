
#include "meshcomponent.h"

#include "core/graphics/rendercontext.h"
#include "core/graphics/sprites/sprite.h"
#include "core/math/vertex.h"

#include "core/entity/entity.h"
#include "core/entity/componentmessages/animationcomponentmessage.h"

#include "components.h"
#include "componentstructs.h"

using namespace Graphics;

MeshComponent::MeshComponent(c2d::Sprite&& sprite):
   Component(ComponentInterface::eMeshComponent),
   mSprite(std::move(sprite))
{
}

MeshComponent::~MeshComponent()
{
}

// - Operations

void MeshComponent::registerComponent(Components& components)
{
	Component::registerComponent(components);

   components.subscribeMessageType(*this, ComponentInterface::ePositionChangedMsg);
   components.subscribeMessageType(*this, ComponentInterface::eUpdatedMsg);
   components.subscribeMessageType(*this, ComponentInterface::eUpdateMsg);
   components.subscribeMessageType(*this, ComponentInterface::eAnimationMsg);
   components.subscribeMessageType(*this, ComponentInterface::eFlipMsg);
   components.subscribeMessageType(*this, ComponentInterface::eScaleMsg);
   components.subscribeMessageType(*this, ComponentInterface::eRenderMsg);
}

void MeshComponent::handleMessage(ComponentMessage& message)
{
   using namespace ComponentInterface;

   switch ( message.getMessageType() )
   {
   case ePositionChangedMsg:
   case eUpdatedMsg:
      {
         mSprite.setTransform(getEntity().getTransform());
      }
      break;
   case eUpdateMsg:
      {
         float delta = *static_cast<const float*>(message.getData());
         update(delta);
      }
      break;
   case eAnimationMsg:
      {
         AnimationComponentMessage& msg = static_cast<AnimationComponentMessage&>(message);
         mSprite.setAnimation(msg.getAnimation());
      }
      break;
   case eFlipMsg:
      mSprite.flip();
      break;
   case eRenderMsg:
      {
         RenderContext* pcontext = static_cast<RenderContext*>(message.getData());
         render(*pcontext);
      }
      break;
      
   default:
      break;
   }
}

void MeshComponent::update(float delta)
{
   mSprite.update(delta);
}

void MeshComponent::render(RenderContext& context) const
{
   context.drawSprite(mSprite);
}
