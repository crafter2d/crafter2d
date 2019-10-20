/***************************************************************************
 *   Copyright (C) 2013 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "ogldevice.h"

#include "core/log/log.h"
#include "core/graphics/blendstatedesc.h"

#include "text/oglglyphprovider.h"
#include "text/oglfont.h"

#include "oglblendstate.h"
#include "ogltexture.h"
#include "oglrendertarget.h"
#include "oglrendercontext.h"
#include "oglindexbuffer.h"
#include "oglvertexbuffer.h"
#include "shaderpath.h"

namespace Graphics
{

OGLDevice::OGLDevice() :
   Device(),
   mFreeTypeLib(nullptr)
{
}

OGLDevice::~OGLDevice()
{
   FT_Done_FreeType(mFreeTypeLib);
}

// - Creation

void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
   if ( type == GL_DEBUG_TYPE_ERROR )
   {
      Log::getInstance().error("GL CALLBACK: type = 0x%x, severity = 0x%x, message = %s\n", type, severity, message);
   }
   //else
      //Log::getInstance().info("GL CALLBACK: type = 0x%x, severity = 0x%x, message = %s\n", type, severity, message);
}

bool OGLDevice::create(GameWindow& window)
{
   Log& log = Log::getInstance();
   GLint units;
   
   int err = gl3wInit();
   if ( err != GL3W_OK )
   {
      log << "Failed to initialize OpenGL\n";
      return false;
   }

   log << "Graphics card:\t\t" << (char*)glGetString(GL_VENDOR) << "\n";
   log << "OpenGL version:\t\t" << (char*)glGetString(GL_VERSION) << ", shader version " << (char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

   if ( !gl3wIsSupported(4, 3) )
   {
      log << "OpenGL version 4.3 is required to run this application.\n";
      return false;
   }

   // make sure that there are enough texture units
   glGetIntegerv (GL_MAX_TEXTURE_IMAGE_UNITS, &units);
   if ( units < 2 )
   {
      log << "Videocard does not support 2 or more texture units.\n";
   }

   log << "Max textures:\t\t" << (int)units << "\n";
   int maxTextureSize = 0;
   glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
   log << "Max texture size:\t" << maxTextureSize << "\n";
  
   // initialize free type
   if ( FT_Init_FreeType(&mFreeTypeLib) != 0 )
      return false;

   // During init, enable debug output
   glEnable(GL_DEBUG_OUTPUT);
   glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
   glDebugMessageCallback(MessageCallback, 0);

   return Device::create(window);
}

void OGLDevice::resize(int width, int height)
{
   C2D_UNUSED(width);
   C2D_UNUSED(height);
}

RenderContext* OGLDevice::createRenderContext()
{
   return new OGLRenderContext();
}

CodePath* OGLDevice::createCodePath()
{
   return new ShaderPath();
}

/// \fn OGLDevice::createVertexBuffer()
/// \brief Creates a vertex buffer for storage of the geometry. When Vertex Buffer Objects are supported, one will
/// be created. A default buffer will be created otherwise.
/// \return A pointer to the vertex buffer instance.
VertexBuffer* OGLDevice::createVertexBuffer()
{
	return new OGLVertexBuffer();
}

IndexBuffer* OGLDevice::createIndexBuffer()
{
   return new OGLIndexBuffer();
}

/// \fn OGLDevice::createTexture()
/// \brief Creates a new empty texture.
/// \return A pointer to the created texture.
Texture* OGLDevice::createTexture(int width, int height, int bytesperpixel)
{
   OGLTexture* ptexture = new OGLTexture();
   ptexture->create(width, height, bytesperpixel);
   return ptexture;
}

Texture* OGLDevice::createTexture(const TextureDescription& desc)
{
   OGLTexture* ptexture = new OGLTexture();
   ptexture->create(desc);
   return ptexture;
}

RenderTarget* OGLDevice::createRenderTarget()
{
   return new OGLRenderTarget();
}

BlendState* OGLDevice::createBlendState(const BlendStateDesc& desc)
{
   GLenum sfactor = toGLBlendState(desc.getSourceFactor());
   GLenum dfactor = toGLBlendState(desc.getDestFactor());
   
   if ( sfactor == GL_INVALID_ENUM || dfactor == GL_INVALID_ENUM )
   {
      return nullptr;
   }

   return new OGLBlendState(sfactor, dfactor, desc.isEnabled());
}

GLenum OGLDevice::toGLBlendState(BlendStateDesc::BlendFactor blendfactor)
{
   GLenum factor = GL_INVALID_ENUM;

   switch ( blendfactor )
   {
      case BlendStateDesc::BS_ONE:
         factor = GL_ONE;
         break;
      case BlendStateDesc::BS_ZERO:
         factor = GL_ZERO;
         break;
      case BlendStateDesc::BS_SRC_COLOR:
         factor = GL_SRC_COLOR;
         break;
      case BlendStateDesc::BS_SRC_INV_COLOR:
         factor = GL_ONE_MINUS_SRC_COLOR;
         break;
      case BlendStateDesc::BS_SRC_ALPHA:
         factor = GL_SRC_ALPHA;
         break;
      case BlendStateDesc::BS_SRC_INV_ALPHA:
         factor = GL_ONE_MINUS_SRC_ALPHA;
         break;
      case BlendStateDesc::BS_DST_COLOR:
         factor = GL_DST_COLOR;
         break;
      case BlendStateDesc::BS_DST_INV_COLOR:
         factor = GL_ONE_MINUS_DST_COLOR;
         break;
      case BlendStateDesc::BS_DST_ALPHA:
         factor = GL_DST_ALPHA;
         break;
      case BlendStateDesc::BS_DST_INV_ALPHA:
         factor = GL_ONE_MINUS_DST_ALPHA;
         break;
      case BlendStateDesc::BS_INVALID:
      default:
         break;
   }
   return factor;
}

Font* OGLDevice::createFont(const String& name)
{
   FT_Face face;
   const std::string file = name.toUtf8() + ".ttf";
   FT_Error error = FT_New_Face(mFreeTypeLib, file.c_str(), 0, &face);
   if ( error != 0 )
   {
      return nullptr;
   }

   OGLGlyphProvider* pprovider = new OGLGlyphProvider(mFreeTypeLib, getContext().getTextRenderer().getGlyphAtlas());
   OGLFont* pfont = new OGLFont(pprovider, face);
   pfont->setFamilyName(name);
   pfont->setSize(24.0f);
   pprovider->initialize(*pfont);

   return pfont;
}

// - Presenting

void OGLDevice::present()
{
}

};
