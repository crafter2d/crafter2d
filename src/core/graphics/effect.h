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
#ifndef EFFECT_H_
#define EFFECT_H_

#include <vector>

#include "core/string/string.h"
#include "core/resource/resourcemanager.h"

class TiXmlElement;

namespace Graphics
{
   class BlendState;
   class CodePath;
   class Device;
   class RenderContext;
   class Texture;
   class VertexInputLayout;
   class UniformBuffer;

   /*!
   @author Jeroen Broekhuizen
   \brief Implements an effect for a collection of triangles.

   This class implements the an effect for use during rendering of objects. It keeps track
   of textures (one diffuse map and extra textures for other stages), shaders and eventually
   texture combiners. Either GLSL or vertex programs are used depending on the target hardware
   (if GLSL is not supported the GLSL code is automatically converted to vertex program ASM
   code).
    */
   class CORE_API Effect
   {
   public:
	                     Effect();
	                     ~Effect();
	
	   bool              load(Graphics::Device& device, const VertexInputLayout& layout, const String& file);
	   void              destroy();
	
	   const TexturePtr  resolveTexture(const String& uniform) const;
      const TexturePtr  findTexture(const String& uniform) const;

      UniformBuffer*    getUniformBuffer(const String& name) const;

      void              enable(Graphics::RenderContext& context) const;
	   void              disable(Graphics::RenderContext& context) const;

      void              updateStateMatrices() const;
	
   private:
     // types
      typedef std::vector<TexturePtr> Textures;

	   bool              processTextures(Graphics::Device& device, const TiXmlElement& effect);
      bool              postprocessTextures();

	   bool              processCode(Graphics::Device& device, const Graphics::VertexInputLayout& layout, const TiXmlElement& effect, const String& path);
      bool              processBlendState(Graphics::Device& device, const TiXmlElement& effect);
		
      String                        name;
      Textures                      mTextures;
      Graphics::CodePath*           mpCodePath;
      Graphics::BlendState*         mpBlendStateEnabled;
      Graphics::BlendState*         mpBlendStateDisabled;
   };
}

#ifdef JENGINE_INLINE
#  include "effect.inl"
#endif

#endif
