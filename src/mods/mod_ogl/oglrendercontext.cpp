
#include "oglrendercontext.h"

#include "GL/gl3w.h"
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
#include "oglrendertarget.h"

using namespace Graphics;

OGLRenderContext::OGLRenderContext():
   mpEffect(nullptr),
   mpCodePath(nullptr),
   mpVertexBuffer(nullptr),
   mpIndexBuffer(nullptr)
{
}

/*
void OGLRenderContext::onViewportChanged(const Viewport& viewport)
{
   glViewport(viewport.getLeft(), viewport.getTop(), viewport.getWidth(), viewport.getHeight());
}
*/

void OGLRenderContext::setRenderTarget(RenderTarget& target)
{
   OGLRenderTarget& ogltarget = static_cast<OGLRenderTarget&>(target);
   glBindFramebuffer(GL_FRAMEBUFFER, ogltarget.getFBO());
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
      glEnable(GL_BLEND);
      glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
      glBlendFuncSeparate(oglstate.getSourceFactor(), oglstate.getDestFactor(), GL_ZERO, GL_ONE);
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

void OGLRenderContext::setTexture(int stage, const Texture& texture)
{
   ASSERT_PTR(mpCodePath);
   texture.enable(*this, stage);
}

void OGLRenderContext::clear()
{
   //const Color& color = getClearColor();

   //glClearColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
   glClearColor(0, 0, 0, 1);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OGLRenderContext::drawPoints(int start, int count)
{
   glDrawArrays(GL_POINTS, start, count);
}

void OGLRenderContext::drawTriangles(int start, int count)
{
   ASSERT_PTR(mpIndexBuffer);
   GLenum type = mpIndexBuffer->getNativeType();
   glDrawElements(GL_TRIANGLES, count, type, (void*)(start * sizeof(unsigned short)));
}

void OGLRenderContext::drawTriangleFan(int start, int count)
{
   C2D_UNUSED(start);
   
   ASSERT_PTR(mpIndexBuffer);

   glDrawElements(GL_TRIANGLE_FAN, count, mpIndexBuffer->getNativeType(), 0);
}

void OGLRenderContext::drawTriangleStrip(int start, int count)
{
   C2D_UNUSED(start);
   
   ASSERT_PTR(mpIndexBuffer);

   glDrawElements(GL_TRIANGLE_STRIP, count, mpIndexBuffer->getNativeType(), 0);
}

