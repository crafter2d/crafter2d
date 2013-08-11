/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "effect.h"
#ifndef JENGINE_INLINE
#  include "effect.inl"
#endif

#include <tinyxml.h>

#include "core/vfs/file.h"
#include "core/log/log.h"

#include "codepath.h"
#include "device.h"
#include "blendstatedesc.h"
#include "rendercontext.h"
#include "texture.h"

namespace Graphics
{

/*!
    \fn Effect::Effect(void)
	 \brief initializes internal member variables (by default texture combiners are not used).
 */
Effect::Effect():
   name(),
   mFile(),
   mTextures(),
   mpCodePath(NULL),
   mpBlendStateEnabled(NULL),
   mpBlendStateDisabled(NULL)
{
}

/*!
    \fn Effect::~Effect(void)
	 \brief releases internal member variables.
 */
Effect::~Effect()
{
	destroy();
}

/*!
    \fn Effect::load(const String& file)
	 \brief Loads an effect file and prepairs the OpenGL textures and shaders. It also
	 sets up pointers to the uniform variables.
	 \retval true the effect file is loaded correctly.
	 \retval false an error occured (look in the log file for a message).
 */
bool Effect::load(Device& device, const VertexInputLayout& layout, const String& file)
{
	Log& log = Log::getInstance();

   mFile = file;
   String path = UTEXT("../shaders/") + file + UTEXT(".xml");

   std::string filename = path.toUtf8();
   TiXmlDocument doc(filename);
	if ( !doc.LoadFile() )
   {
      log.error("Effect.load: can not load '%s'", filename.c_str());
		return false;
	}

	// get the root element from the file
	const TiXmlElement* effect = static_cast<TiXmlElement*>(doc.FirstChild("effect"));
	if ( effect == NULL )
   {
      log.error("Effect.load: %s is not an effect file.", filename.c_str());
		return false;
	}

   // save the name (if supplied)
   name = effect->Attribute ("name");

	// try to load in the textures
	if ( !processCode(device, layout, *effect, UTEXT("../shaders/")) 
     || !processBlendState(device, *effect) )
   {
		return false;
   }

	return true;
}

/*!
    \fn Effect::destroy()
	 \brief Releases the codepath and the data for the stages. After calling this function you must
	 call load before enabling it again.
 */
void Effect::destroy ()
{
   if( mpCodePath != NULL )
   {
		// release the path
		mpCodePath->release ();
		delete mpCodePath;
		mpCodePath = NULL;
	}

	mTextures.clear ();
}

/*!
    \fn Effect::processCode(Graphics::Device& device, const Graphics::VertexInputLayout& layout, const TiXmlElement* effect, const String& path)
	 \brief Loads in the shaders (if any) from the file and creates the code path. In case GLSL isn't
	 supported it first checks the existence of a vertex program. If that doesn't exist the GLSL counter
	 part is loaded and automatically converted.
	 \returns true when no errors are detected, false otherwise.
 */
bool Effect::processCode(Graphics::Device& device, const Graphics::VertexInputLayout& layout, const TiXmlElement& effect, const String& path)
{
   const char* vertex = NULL, *fragment = NULL;

   const TiXmlElement* pcode_part = static_cast<const TiXmlElement*>(effect.FirstChild("code"));
   if ( pcode_part == NULL )
   {
      Log::getInstance().error("Effect.processCode: effect file doesn't contain a code block!");
		return false;
   }

   // load the vertex shader
   const TiXmlElement* pshader_part = static_cast<const TiXmlElement*>(pcode_part->FirstChild("vertex"));
   if ( pshader_part != NULL )
   {
      const TiXmlText* psource = static_cast<const TiXmlText*>(pshader_part->FirstChild());
      ASSERT_PTR(psource);

	   vertex = psource->Value ();
   }

   // load the fragment shader
   pshader_part = static_cast<const TiXmlElement*>(pcode_part->FirstChild("fragment"));
   if ( pshader_part != NULL )
   {
	   // see if there is a GLSL shader available
      const TiXmlText* psource = static_cast<const TiXmlText*>(pshader_part->FirstChild());
      ASSERT_PTR(psource);

	   fragment = psource->Value ();
   }

   // files should be in same directory
   String vertexfile   = path + String::fromUtf8(vertex);
   String fragmentfile = path + String::fromUtf8(fragment);

   // now load the codepath
   mpCodePath = device.createCodePath();
   if ( mpCodePath == NULL || !mpCodePath->load(layout, vertexfile, fragmentfile) )
       return false;
   
	return true;
}

/// \fn Effect::processBlendState(Device& device, const TiXmlElement& effect)
/// \brief Loads in any blendstate that is described in the effect
bool Effect::processBlendState(Graphics::Device& device, const TiXmlElement& effect)
{
   const TiXmlElement* pblend_part = static_cast<const TiXmlElement*>(effect.FirstChild("blend"));
   if ( pblend_part != NULL )
   {
      using namespace Graphics;

      String strsource = String::fromUtf8(pblend_part->Attribute("source"));
      String strdest   = String::fromUtf8(pblend_part->Attribute("dest"));
     
      BlendStateDesc descenabled(BlendStateDesc::fromString(strsource), BlendStateDesc::fromString(strdest), true);
      mpBlendStateEnabled = device.createBlendState(descenabled);

      BlendStateDesc descdisabled(BlendStateDesc::BS_ONE, BlendStateDesc::BS_ZERO, false);
      mpBlendStateDisabled = device.createBlendState(descdisabled);
   }

   BlendStateDesc desc(BlendStateDesc::BS_SRC_ALPHA, BlendStateDesc::BS_SRC_INV_ALPHA, true);
   mpBlendStateEnabled = device.createBlendState(desc);
   
   return true;
}

UniformBuffer* Effect::getUniformBuffer(const String& name) const
{
   ASSERT_PTR(mpCodePath);
   return mpCodePath->getUniformBuffer(name);
}

void Effect::setTexture(int stage, const TexturePtr& texture)
{
   TexInfo info = { texture, stage };
   mTextures.push_back(info);
}

/*!
    \fn Effect::enable()
	 \brief Enables the textures, shaders and if neccessary the texture combiners.
 */
void Effect::enable(RenderContext& context) const
{
   context.setBlendState(*mpBlendStateEnabled);
   
   mpCodePath->enable(context);

   for ( Textures::size_type s = 0; s < mTextures.size(); ++s )
   {
	   const TexInfo& info = mTextures[s];
      mpCodePath->bindTexture(context, info.stage, *(info.texture));
   }
}

void Effect::render(RenderContext& context, int vertcount)
{
   enable(context);

   context.drawTriangles(0, vertcount);

   mTextures.clear();
}

} // end namespace