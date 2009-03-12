/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef SHADER_H
#define SHADER_H

#define _STLSOFT_NO_ASSERT

#include <GL/GLee.h>
#include <vector>

/**
@author Jeroen Broekhuizen
\brief Base class for the shaders. It profides basic functionality needed to load and compile a vertex- and fragment. Don't
create a instance of this class but rather of VertexShader or Fragment shader.
shader from file.
*/
class Shader{
public:
                  Shader();
   virtual        ~Shader();

	void           release();
   bool           compile( const char* filename );
   bool           compile( const char* source, int len );

   void           handle( GLhandleARB shader );
   GLhandleARB    handle() const;

private:
   GLcharARB*     load( const char* file, int& length );

protected:
	/// \brief The OpenGL shader object handle
	GLhandleARB shader;
};

/**
@author Jeroen Broekhuizen
\brief Implements the vertex shader. With vertex shaders you can customize the rendering pipeline to your own needs and
offload per vertex math to the graphics card and use the CPU for other calculations.
*/
class VertexShader: public Shader{
public:
    VertexShader();
};

/**
@author Jeroen Broekhuizen
\brief Implements the fragment shader object. With fragment shaders you can make custom fragment effects not possible in the
default rendering pipeline. Note that fragment shaders are not available on all systems. Only the latest graphics
cards (GeForce FX and higher) supports them.
*/
class FragmentShader: public Shader{
public:
    FragmentShader();
};

/**
@author Jeroen Broekhuizen
\brief The shader object class. It keeps track of the OpenGL program which can hold vertex- and fragment shaders. With this class
you can use your shaders in the game.
*/
class ShaderObject{
public:
               ShaderObject();
               ~ShaderObject();

   bool        create();
	void        release();

   void        addShader(Shader* shader);
   bool        link();

   void        enable() const;
   void        disable() const;

   GLhandleARB handle() const;
   bool        valid() const;

   GLint       getAttribute( const char* name ) const;
   GLint       getUniformLocation( const char* name ) const;

   void        setUniform1i( GLint index, int i );
	void        setUniform3f( GLint index, float x, float y, float z );
   void        setUniform4f( GLint index, float x, float y, float z, float w );

private:
   std::vector<Shader*> shaders;
   GLhandleARB program;
};

#ifdef JENGINE_INLINE
#  include "shader.inl"
#endif

#endif
