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
#include "core/defines.h"

namespace Graphics
{

/// \fn Shader::handle( GLhandleARB _shader )
/// \brief Set a new handle of the shader.
INLINE void Shader::handle(GLuint shader)
{
   if ( mShader != NULL )
      release();

   mShader = shader;
}

/// \fn Shader::handle()
/// \brief Use this function to get the internal handle to the shader object. It is recommended for advanced users only.
/// \returns Handle to the shader object (either a vertex- or fragment shader).
INLINE GLuint Shader::handle() const
{
	return mShader;
}

};