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
#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

class String;

namespace Graphics
{
   class ShaderUniformBlock;
   class Texture;

   /**
   @author Jeroen Broekhuizen
   \brief Base class for the shaders. It profides basic functionality needed to load and compile a vertex- and fragment. Don't
   create a instance of this class but rather of VertexShader or Fragment shader.
   shader from file.
   */
   class Shader
   {
   public:
              Shader();
      virtual ~Shader();

    // get/set
      GLuint handle() const;

    // compilation
      bool    compile(const String& filename);
      bool    compile(const char* source, int len);
      void    release();

   protected:
    // get/set
      void   handle(GLuint shader);

   private:
      GLchar* load(const String& file, int& length);

	   /// \brief The OpenGL shader object handle
	   GLuint mShader;
   };

   /**
   @author Jeroen Broekhuizen
   \brief Implements the vertex shader. With vertex shaders you can customize the rendering pipeline to your own needs and
   offload per vertex math to the graphics card and use the CPU for other calculations.
   */
   class VertexShader: public Shader
   {
   public:
       VertexShader();
   };

   class GeometryShader : public Shader
   {
   public:
      GeometryShader();
   };

   /**
   @author Jeroen Broekhuizen
   \brief Implements the fragment shader object. With fragment shaders you can make custom fragment effects not possible in the
   default rendering pipeline. Note that fragment shaders are not available on all systems. Only the latest graphics
   cards (GeForce FX and higher) supports them.
   */
   class FragmentShader: public Shader
   {
   public:
       FragmentShader();
   };

};

#ifdef JENGINE_INLINE
#  include "shader.inl"
#endif

#endif
