/***************************************************************************
 *   Copyright (C) 2012 by Jeroen Broekhuizen                              *
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
#include "core/defines.h"

namespace Graphics
{

/// \fn ShaderObject::addShader(const Shader& shader)
/// \brief Adds a shader to the program. Do not release this shader untill you called the link function (also happens if
/// the shader moves out of its scope!).
/// \param shader Reference to the shader you want to add, this can either be a vertex- or fragment shader.
INLINE void ShaderObject::addShader(Shader* pshader)
{
	shaders.push_back(pshader);
}

INLINE GLhandleARB ShaderObject::handle() const
{ 
   return program;
}

/// \fn ShaderObject::enable()
/// \brief Enables this object in the rendering pipeline. All geometry that is rendered will be translate by this object untill
/// disable() is called.
INLINE void ShaderObject::enable() const
{
	glUseProgramObjectARB (program);
}

/// \fn ShaderObject::disable()
/// \brief Stops using this object and restores original OpenGL rendering pipeline.
INLINE void ShaderObject::disable() const
{
	glUseProgramObjectARB (0);
}

INLINE void ShaderObject::setStateUniform(int index)
{
}

INLINE void ShaderObject::setUniform1i (GLint index, int i)
{
	glUniform1iARB (index, i);
}

INLINE void ShaderObject::setUniform3f (GLint index, float x, float y, float z)
{
	glUniform3fARB (index, x, y, z);
}

/// \fn ShaderObject::setUniform4f(const char* name, float x, float y, float z, float w)
/// \brief Set the values of a \em uniform variable in the shader.
/// \param[in] name the name of the uniform parameter
/// \param[in] x the X coordinate
/// \param[in] y the Y coordinate
/// \param[in] z the Z coordinate
/// \param[in] w the W coordinate
INLINE void ShaderObject::setUniform4f(GLint index, float x, float y, float z, float w)
{
	glUniform4fARB (index, x, y, z, w);
}

};
