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
#include "texture.h"

class CodePath;
class TiXmlElement;

/*!
@author Jeroen Broekhuizen
\brief Implements an effect for a collection of triangles.

This class implements the an effect for use during rendering of objects. It keeps track
of textures (one diffuse map and extra textures for other stages), shaders and eventually
texture combiners. Either GLSL or vertex programs are used depending on the target hardware
(if GLSL is not supported the GLSL code is automatically converted to vertex program ASM
code).
 */
class Effect
{
	struct TexStage {
		TexturePtr tex;
      String uniform;
		int index;
		GLint combiner;
		GLint source0;
		GLint source1;
	};
	
public:
	                  Effect();
	                  ~Effect();
	
	bool              load(const String& file);
	void              destroy();
	
	const TexturePtr  resolveTexture(const String& uniform) const;
   const TexturePtr  findTexture(const String& uniform) const;

         bool        hasPath() const;
   const CodePath&   getPath() const;
         void        setPath(CodePath* path);
	
	void              enable () const;
	void              disable () const;
	
private:
  // types
   typedef std::vector<TexStage> Stages;

	bool              processTextures(const TiXmlElement& effect);
	bool              processCode(const TiXmlElement& effect, const String& path);
	bool              processCombiners(const TiXmlElement& shader_part);
	bool              postprocessTextures();
	
	GLint             getCombinerValue(const char* str);
	GLint             getSourceValue(const char* str);
	
   String      name;
   Stages      stages;
   CodePath*   mCodePath;
   bool        useCombiners;  
};

#ifdef JENGINE_INLINE
#  include "effect.inl"
#endif

#endif
