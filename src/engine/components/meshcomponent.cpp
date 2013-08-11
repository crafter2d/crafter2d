
#include "meshcomponent.h"
#ifndef JENGINE_INLINE
#  include "meshcomponent.inl"
#endif

#include "core/graphics/rendercontext.h"
#include "core/math/vertex.h"

#include "engine/animator.h"
#include "engine/texturecoordinate.h"
#include "engine/sprites/sprite.h"

#include "components.h"
#include "componentmessage.h"
#include "componentstructs.h"

using namespace Graphics;

MeshComponent::MeshComponent():
   Component(ComponentInterface::eMeshComponent),
   mTransform(),
   mpAnimator(NULL),
   mpSprite(NULL)
{
}

MeshComponent::~MeshComponent()
{
   delete mpAnimator;
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

	components.subscribeMessageType(*this, ComponentInterface::ePositionChangedMsg);
   components.subscribeMessageType(*this, ComponentInterface::eUpdateMsg);
   components.subscribeMessageType(*this, ComponentInterface::eRenderMsg);
}

void MeshComponent::handleMessage(ComponentMessage& message)
{
	using namespace ComponentInterface;

	switch ( message.getMessageType() )
	{
	case ePositionChangedMsg:
		{
         PositionInfo* pinfo = (PositionInfo*) message.getData();
         mTransform = pinfo->transform;
		}
      break;
   case eUpdateMsg:
      {
         float* pdelta = (float*)message.getData();
         update(*pdelta);
      }
      break;
   case eRenderMsg:
      {
         RenderContext* pcontext = static_cast<RenderContext*>(message.getData());
         render(*pcontext);
      }
		break;
	}
}

void MeshComponent::update(float delta)
{
   /*
   if ( mpAnimator != NULL && mpAnimator->animate(delta) )
   {
      updateBuffers();
   }
   */
}

void MeshComponent::updateBuffers(RenderContext& context)
{
   const TextureCoordinate& texcoord = mpAnimator->getTextureCoordinate();

   /*
   PTVertex* pdata = reinterpret_cast<PTVertex*>(mpVertexBuffer->lock(context));

   pdata[0].pos.set(-mHalfSize.width, -mHalfSize.height);
   pdata[0].tex = texcoord.getTopLeft();

   pdata[1].pos.set(mHalfSize.width, -mHalfSize.height);
   pdata[1].tex = texcoord.getTopRight();

   pdata[2].pos.set(mHalfSize.width, mHalfSize.height);
   pdata[2].tex = texcoord.getBottomRight();

   pdata[3].pos.set(-mHalfSize.width, mHalfSize.height);
   pdata[3].tex = texcoord.getBottomLeft();

   mpVertexBuffer->unlock(context); */
}

void MeshComponent::render(RenderContext& context) const
{
   /*
   context.setEffect(mEffect);
   context.setVertexBuffer(*mpVertexBuffer);
   context.setIndexBuffer(*mpIndexBuffer);
   context.drawTriangleFan(0, 4);
   */
}
