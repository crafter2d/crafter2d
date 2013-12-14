
#include "oglrendercontext.h"

#include <GL/GLee.h>
#include <GL/glu.h>

#include "core/graphics/codepath.h"
#include "core/graphics/texture.h"
#include "core/graphics/viewport.h"
#include "core/graphics/uniformbuffer.h"
#include "core/math/color.h"
#include "core/math/xform.h"
#include "core/math/matrix4.h"

#include "oglblendstate.h"
#include "oglindexbuffer.h"
#include "oglvertexbuffer.h"


using namespace Graphics;

OGLRenderContext::OGLRenderContext():
   mpEffect(NULL),
   mpCodePath(NULL),
   mpVertexBuffer(NULL),
   mpIndexBuffer(NULL)
{
}

void OGLRenderContext::onViewportChanged(const Viewport& viewport)
{
   glViewport(viewport.getLeft(), viewport.getTop(), viewport.getWidth(), viewport.getHeight());
}

void OGLRenderContext::setCodePath(CodePath& path)
{
   mpCodePath = &path;
   path.enable(*this);
}

void OGLRenderContext::setBlendState(const BlendState& state)
{
   const OGLBlendState& oglstate = static_cast<const OGLBlendState&>(state);
   if ( oglstate.isEnabled() )
   {
      glBlendFunc(oglstate.getSourceFactor(), oglstate.getDestFactor());
      glEnable(GL_BLEND);
   }
   else
   {
      glDisable(GL_BLEND);
   }
}

void OGLRenderContext::setVertexBuffer(const VertexBuffer& buffer)
{
   mpVertexBuffer = static_cast<const OGLVertexBuffer*>(&buffer);
   mpVertexBuffer->enable(*this);
}

void OGLRenderContext::setIndexBuffer(const IndexBuffer& buffer)
{
   mpIndexBuffer = static_cast<const OGLIndexBuffer*>(&buffer);
   mpIndexBuffer->enable(*this);
}

void OGLRenderContext::setUniformBuffer(const UniformBuffer& buffer)
{
   buffer.enable(*this);
}

void OGLRenderContext::setTexture(int stage, const Texture& texture)
{
   ASSERT_PTR(mpCodePath);
   //mpCodePath->bindTexture(*this, stage, texture);
   texture.enable(*this, stage);
}

void OGLRenderContext::clear()
{
   const Color& color = getClearColor();

   glClearColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OGLRenderContext::drawTriangles(int start, int count)
{
   ASSERT_PTR(mpIndexBuffer);
   GLenum type = mpIndexBuffer->getNativeType();
   glDrawElements(GL_TRIANGLES, count, type, (void*)(start * sizeof(unsigned short)));
}

void OGLRenderContext::drawTriangleFan(int start, int count)
{
   ASSERT_PTR(mpIndexBuffer);

   glDrawElements(GL_TRIANGLE_FAN, count, mpIndexBuffer->getNativeType(), 0);
}

void OGLRenderContext::drawTriangleStrip(int start, int count)
{
   ASSERT_PTR(mpIndexBuffer);

   glDrawElements(GL_TRIANGLE_STRIP, count, mpIndexBuffer->getNativeType(), 0);
}

