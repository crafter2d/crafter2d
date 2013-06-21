
#include "oglrendercontext.h"

#include <GL/GLee.h>
#include <GL/glu.h>

#include "core/graphics/effect.h"
#include "core/graphics/viewport.h"
#include "core/math/color.h"
#include "core/math/xform.h"
#include "core/math/matrix4.h"

#include "oglblendstate.h"
#include "oglindexbuffer.h"
#include "oglvertexbuffer.h"

using namespace Graphics;

OGLRenderContext::OGLRenderContext():
   mpEffect(NULL),
   mpVertexBuffer(NULL),
   mpIndexBuffer(NULL)
{
}

void OGLRenderContext::onViewportChanged(const Viewport& viewport)
{
   glViewport(viewport.getLeft(), viewport.getTop(), viewport.getWidth(), viewport.getHeight());
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

void OGLRenderContext::setEffect(const Effect& effect)
{
   mpEffect = &effect;
   mpEffect->enable(*this);
}

void OGLRenderContext::setVertexBuffer(const VertexBuffer& buffer)
{
   mpVertexBuffer = static_cast<const OGLVertexBuffer*>(&buffer);
   mpVertexBuffer->enable();
}

void OGLRenderContext::setIndexBuffer(const IndexBuffer& buffer)
{
   mpIndexBuffer = static_cast<const OGLIndexBuffer*>(&buffer);
   mpIndexBuffer->enable();
}

void OGLRenderContext::setOrthoProjection()
{
   GLint viewport[4];
   glGetIntegerv(GL_VIEWPORT, viewport);

   int width = viewport[2] - viewport[0];
	int height = viewport[3] - viewport[1];

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0, width, height, 0);
}

void OGLRenderContext::setObjectMatrix(const XForm& matrix)
{
   mObjectMatrix = matrix;

   updateViewMatrix();
}

void OGLRenderContext::setWorldMatrix(const XForm& matrix)
{
   mWorldMatrix = matrix;

   updateViewMatrix();
}

void OGLRenderContext::updateViewMatrix()
{
   XForm matrix = mWorldMatrix * mObjectMatrix;
   matrix.asOpenGL(matogl);

   glMatrixMode(GL_MODELVIEW);
   glLoadMatrixf(matogl);

   if ( mpEffect != NULL )
   {
      mpEffect->updateStateMatrices();
   }
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
   glDrawElements(GL_TRIANGLES, count, type, 0);
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

