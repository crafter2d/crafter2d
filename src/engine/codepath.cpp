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
#include "codepath.h"

#include <algorithm>
#include <tinyxml.h>

#include "core/autoptr.h"
#include "core/defines.h"
#include "core/log/log.h"

#include "opengl.h"
#include "texture.h"

ShaderPath::ShaderPath():
   CodePath(),
   shader()
{
}

/*!
    \fn ShaderPath::load (const char* vertex, const char* fragment)
	\brief Loads in a vertex and fragment shader.

	This function loads the supplied vertex and fragment shaders into the shader
	object of this path. Both parameters are optional and maybe NULL. The shader
	files should contain valid GLSL code, otherwise an linking error message will
	be saved in the log file.

	\param vertex File name of the vertex shader (may be NULL).
	\param fragment File name of the fragment shader (may be NULL).
	\retval true the shader objects have been successfully linked.
	\retval false otherwise, consult the log file for compiler/linker specific errors.
 */
bool ShaderPath::load(const std::string& vertex, const std::string& fragment)
{
	Log& log = Log::getInstance ();
	shader.create();

	// try to load and add the vertex shader
   if ( !vertex.empty() )
   {
		AutoPtr<VertexShader> vs = new VertexShader();
      if ( !vs->compile(vertex.c_str()) )
      {
         log.error("GLSLPath.load: Failed to load or compile vertex shader '%s'", vertex);
			return false;
		}

      shader.addShader(vs.release());
	}

	// try to load and add the fragment shader
   if ( !fragment.empty() )
   {
		AutoPtr<FragmentShader> fs = new FragmentShader();
      if ( !fs->compile(fragment.c_str()) )
      {
         log.error("GLSLPath.load: Failed to load or compile fragment shader '%s'", fragment);
			return false;
		}

      shader.addShader(fs.release());
	}

	// link the shader
	return shader.link();
}

void ShaderPath::release ()
{
	shader.release ();
}

void ShaderPath::enable () const
{
	shader.enable();
}

void ShaderPath::disable () const
{
	shader.disable();
}

CodePath::PathType ShaderPath::getType() const
{
   return CodePath::EGLSL;
}

int ShaderPath::getUniformLocation (const char* name) const
{
	return shader.getUniformLocation(name);
}

void ShaderPath::setUniform1i (int index, int val)
{
	shader.setUniform1i(index, val);
}

void ShaderPath::setUniform3f (int index, float x, float y, float z)
{
	shader.setUniform3f (index, x, y, z);
}

void ShaderPath::setUniform4f (int index, float x, float y, float z, float w)
{
	shader.setUniform4f (index, x, y, z, w);
}

ProgramPath::ProgramPath():
   CodePath(),
   program()
{
}

ProgramPath::~ProgramPath()
{
}

bool ProgramPath::load (const std::string& vertex, const std::string& fragment)
{
	// forget about the fragment shader, it is not supported
	if ( !program.compile(vertex.c_str()) )
		return false;

	return true;
}

void ProgramPath::release ()
{
	program.release ();
}

void ProgramPath::enable () const
{
	program.enable();
}

void ProgramPath::disable () const
{
	program.disable();
}

CodePath::PathType ProgramPath::getType() const
{
   return CodePath::EGLSL;
}

int ProgramPath::getUniformLocation (const char* name) const
{
	return program.getUniformLocation(name);
}

void ProgramPath::setUniform1i (int index, int val)
{
}

void ProgramPath::setUniform3f (int index, float x, float y, float z)
{
}

void ProgramPath::setUniform4f (int index, float x, float y, float z, float w)
{
    glProgramLocalParameter4fARB (GL_VERTEX_PROGRAM_ARB, index, x, y, z, w);
}

