
#include "meshcomponent.h"
#ifndef JENGINE_INLINE
#  include "meshcomponent.inl"
#endif

#include "core/graphics/rendercontext.h"
#include "core/graphics/sprites/sprite.h"
#include "core/math/vertex.h"

#include "components.h"
#include "componentmessage.h"
#include "componentstructs.h"

using namespace Graphics;

MeshComponent::MeshComponent():
   Component(ComponentInterface::eMeshComponent),
   mTransform(),
   mpSprite(NULL)
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
         PositionInfo* pinfo = static_cast<PositionInfo*>(message.getData());
         mpSprite->setTransform(pinfo->transform);
		}
      break;
   case eUpdateMsg:
      {
         float* pdelta = static_cast<float*>(message.getData());
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
   mpSprite->update(delta);
}

void MeshComponent::render(RenderContext& context) const
{
   context.drawSprite(*mpSprite);
}
