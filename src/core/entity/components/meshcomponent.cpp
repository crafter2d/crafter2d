
#include "meshcomponent.h"

#include "core/graphics/rendercontext.h"
#include "core/graphics/sprites/sprite.h"
#include "core/math/vertex.h"

#include "core/entity/entity.h"
#include "core/entity/componentmessages/animationcomponentmessage.h"

#include "components.h"
#include "componentstructs.h"

using namespace Graphics;

MeshComponent::MeshComponent(c2d::Sprite* psprite):
   Component(ComponentInterface::eMeshComponent),
   mTransform(),
   mpSprite(psprite)
{
}

MeshComponent::~MeshComponent()
{
   delete mpSprite;
}

// - Get/set

const Vector& MeshComponent::getPosition() const
{
   return mTransform.getPosition();
}

float MeshComponent::getAngle() const
{
   return mTransform.getAngle();
}

// - Operations

void MeshComponent::registerComponent(Components& components)
{
	Component::registerComponent(components);

   components.subscribeMessageType(*this, ComponentInterface::eUpdatedMsg);
   components.subscribeMessageType(*this, ComponentInterface::eUpdateMsg);
   components.subscribeMessageType(*this, ComponentInterface::eAnimationMsg);
   components.subscribeMessageType(*this, ComponentInterface::eFlipMsg);
   components.subscribeMessageType(*this, ComponentInterface::eRenderMsg);
}

void MeshComponent::handleMessage(ComponentMessage& message)
{
   using namespace ComponentInterface;

   switch ( message.getMessageType() )
   {
   case eUpdatedMsg:
      {
         mpSprite->setTransform(getEntity().getTransform());
      }
      break;
   case eUpdateMsg:
      {
         float* pdelta = static_cast<float*>(message.getData());
         update(*pdelta);
      }
      break;
   case eAnimationMsg:
      {
         AnimationComponentMessage& msg = static_cast<AnimationComponentMessage&>(message);
         mpSprite->setAnimation(msg.getAnimation());
      }
      break;
   case eFlipMsg:
      mpSprite->flip();
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
   mpSprite->update(delta);
}

void MeshComponent::render(RenderContext& context) const
{
   context.drawSprite(*mpSprite);
}
