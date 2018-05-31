
#include "effecttechnique.h"

#include "core/graphics/codepath.h"
#include "core/graphics/device.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/vertexbuffer.h"
#include "core/defines.h"

namespace Graphics
{

EffectTechnique::EffectTechnique():
   mpCodePath(nullptr)
{
}

EffectTechnique::~EffectTechnique()
{
   if( mpCodePath != nullptr )
   {
		// release the path
		delete mpCodePath;
		mpCodePath = nullptr;
	}
}

// - Get/set

void EffectTechnique::setCodePath(CodePath* ppath)
{
   mpCodePath = ppath;
}

// - Query

UniformBuffer* EffectTechnique::createUniformBuffer(const String& name) const
{
   ASSERT_PTR(mpCodePath);
   return mpCodePath->getUniformBuffer(name);
}

VertexBuffer* EffectTechnique::createVertexBuffer(Device& device, int length, int usage)
{
   VertexBuffer* presult = device.createVertexBuffer();
   if ( presult != nullptr )
   {
      ASSERT_PTR(mpCodePath);
      presult->create(mpCodePath->getVertexLayout(), length, usage);
   }
   return presult;
}

// - Operations

void EffectTechnique::enable(RenderContext& context)
{
   context.setCodePath(*mpCodePath);
}

void EffectTechnique::setConstantBuffer(RenderContext& context, int index, const UniformBuffer& buffer)
{
   mpCodePath->setConstantBuffer(context, index, buffer);
}

} // namespace Graphics
