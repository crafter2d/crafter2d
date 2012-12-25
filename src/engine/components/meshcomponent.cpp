
#include "meshcomponent.h"
#ifndef JENGINE_INLINE
#  include "meshcomponent.inl"
#endif

#include "core/graphics/device.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/indexbuffer.h"
#include "core/graphics/vertexbuffer.h"
#include "core/math/vertex.h"

#include "engine/animator.h"
#include "engine/texturecoordinate.h"

#include "components.h"
#include "componentmessage.h"
#include "componentstructs.h"

using namespace Graphics;

struct PTVertex
{
   Vertex pos;
   Vertex tex;
};

MeshComponent::MeshComponent():
   Component(ComponentInterface::eMeshComponent),
   mpAnimator(NULL),
   mEffect(),
   mEffectName(),
   mpVertexBuffer(NULL),
   mVertexFormat(VertexBuffer::eXY | VertexBuffer::eTex0),
   mSize()
{
}

MeshComponent::~MeshComponent()
{
   delete mpAnimator;
}

void MeshComponent::initialize(Device& device)
{
   mHalfSize.width = mSize.width / 2.0f;
   mHalfSize.height = mSize.height / 2.0f;

   if ( mEffect.load(device, mEffectName) )
   {
      mpVertexBuffer = device.createVertexBuffer();
      mpVertexBuffer->create(4, VertexBuffer::eWriteOnly | VertexBuffer::eDynamic, mVertexFormat);

      short indices[] = { 0, 1, 2, 3 };

      mpIndexBuffer = device.createIndexBuffer();
      mpIndexBuffer->create(IndexBuffer::eShort, 6, indices);

      if ( mpAnimator != NULL )
      {
         const TexturePtr tex = mEffect.findTexture("diffuseMap");
         ASSERT(tex.isValid());

         mpAnimator->initialize(*tex, mSize);
      }

      updateBuffers();
   }
}

void MeshComponent::registerComponent(Components& components)
{
	Component::registerComponent(components);

	components.subscribeMessageType(*this, ComponentInterface::ePositionChangedMsg);
   components.subscribeMessageType(*this, ComponentInterface::eUpdateMsg);
   components.subscribeMessageType(*this, ComponentInterface::eRenderMsg);
}

void MeshComponent::handleMessage(const ComponentMessage& message)
{
	using namespace ComponentInterface;

	switch ( message.getMessageType() )
	{
	case ePositionChangedMsg:
		{
			Matrix4* pinfo = (Matrix4*) message.getData();
         mTransform = *pinfo;
		}
   case eUpdateMsg:
      {
         float* pdelta = (float*)message.getData();
         update(*pdelta);
      }
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
   if ( mpAnimator != NULL && mpAnimator->animate(delta) )
   {
      updateBuffers();
   }
}

void MeshComponent::updateBuffers()
{
   TextureCoordinate texcoord = mpAnimator->getTextureCoordinate();

   PTVertex* pdata = reinterpret_cast<PTVertex*>(mpVertexBuffer->lock(mVertexFormat));

   pdata[0].pos.set(-mHalfSize.width, -mHalfSize.height);
   pdata[0].tex = texcoord.getTopLeft();

   pdata[1].pos.set(-mHalfSize.width, mHalfSize.height);
   pdata[1].tex = texcoord.getBottomLeft();

   pdata[2].pos.set(mHalfSize.width, mHalfSize.height);
   pdata[2].tex = texcoord.getBottomRight();

   pdata[3].pos.set(mHalfSize.width, -mHalfSize.height);
   pdata[3].tex = texcoord.getTopRight();

   mpVertexBuffer->unlock();
}

void MeshComponent::render(RenderContext& context)
{
   mEffect.enable(context);

   context.setWorldMatrix(mTransform);
   context.setVertexBuffer(*mpVertexBuffer);
   context.setIndexBuffer(*mpIndexBuffer);
   
   context.drawTriangleStrip(0, 4);

   mEffect.disable(context);
}
