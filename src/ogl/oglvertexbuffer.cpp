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

#include "oglvertexbuffer.h"

#include <GL/GLee.h>

#include "core/graphics/vertexinputlayout.h"
#include "core/graphics/vertexinputelement.h"
#include "core/log/log.h"
#include "core/defines.h"

namespace Graphics
{

#define FLAG_LOCKED		8192

/*!
    \fn VertexBufferObject::VertexBufferObject()
	 \brief Initializes member variables
 */
OGLVertexBuffer::OGLVertexBuffer()
 : VertexBuffer(),
   mVAO(0),
   mBuffer(0),
   locked(false)
{
}

/*!
    \fn VertexBufferObject::~VertexBufferObject()
	 \brief Releases member variables
 */
OGLVertexBuffer::~OGLVertexBuffer()
{
	release ();
}

/*!
    \fn VertexBufferObject::create(int length, int usage)
 */
bool OGLVertexBuffer::create(const VertexInputLayout& layout, int length, int usage)
{
   if ( !VertexBuffer::create(layout, length, usage) )
      return false;

	GLuint flag = 0;

	// see what usage is required on this buffer
	if ( IS_SET(usage, eStatic) )
   {
		if ( IS_SET(usage, eWriteOnly) )
			flag = GL_STATIC_DRAW;
		else if ( IS_SET(usage, eRead) )
			flag = GL_STATIC_READ;
		else
			flag = GL_STATIC_COPY;
	}
	else if ( IS_SET(usage, eDynamic) )
   {
		if ( IS_SET(usage, eWriteOnly) )
			flag = GL_DYNAMIC_DRAW;
		else if ( IS_SET(usage, eRead) )
			flag = GL_DYNAMIC_READ;
		else
			flag = GL_DYNAMIC_COPY;
	}
	else if ( IS_SET(usage, eStream) )
   {
		if ( IS_SET(usage, eWriteOnly) )
			flag = GL_STREAM_DRAW;
		else if ( IS_SET(usage, eRead) )
			flag = GL_STREAM_READ;
		else
			flag = GL_STREAM_COPY;
	}
	else 
   {
      Log::getInstance().error("VBO Error - Invalid usage flag.\n");
      return false;
   }

   setupIndices(layout);

   glGenVertexArrays(1, &mVAO);
   glBindVertexArray(mVAO);

   glGenBuffers(1, &mBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
   glBufferData(GL_ARRAY_BUFFER, layout.getStride() * length, 0, flag);

   for ( int i = layout.getSize() - 1; i >= 0; --i )
   {
      const VertexInputElement& field = layout[i];

      glVertexAttribPointer(field.index, field.size, GL_FLOAT, GL_FALSE, layout.getStride(), (void*)(field.pos));
      glEnableVertexAttribArray(field.index);
   }

	return true;
}

/*!
    \fn VertexBufferObject::release()
 */
void OGLVertexBuffer::release()
{
   glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &mBuffer);

   glBindVertexArray(0);
   glDeleteVertexArrays(1, &mVAO);

   mVAO = mBuffer = 0;

	VertexBuffer::release();
}

/*!
    \fn VertexBufferObject::lock(RenderContext& context)
 */
float* OGLVertexBuffer::lock(RenderContext& context)
{
   glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
	float* pointer = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (pointer != NULL)
		locked = true;
	return pointer;
}

/*!
    \fn VertexBufferObject::unlock()
 */
void OGLVertexBuffer::unlock(RenderContext& context)
{
	if (locked) {
		glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		locked = false;
	}
}

/*!
    \fn VertexBufferObject::enable()
 */
void OGLVertexBuffer::enable(RenderContext& context) const
{
   glBindVertexArray(mVAO);
}

/*!
    \fn VertexBufferObject::disable()
 */
void OGLVertexBuffer::disable(RenderContext& context) const
{
   glBindVertexArray(0);
}

// - Operations

void OGLVertexBuffer::setupIndices(const VertexInputLayout& layout)
{
   GLint program ;
   glGetIntegerv(GL_CURRENT_PROGRAM, &program);

   for ( int index = 0; index < layout.getSize(); ++index )
   {
      VertexInputElement& field = const_cast<VertexInputElement&>(layout[index]);
      switch ( field.flags )
      {
         case INPUT_XY:
            field.index = glGetAttribLocation(program, "position");
			   break;
		   case INPUT_XYZ:
            field.index = 0;
			   break;
		   case INPUT_XYZW:
            field.index = 0;
			   break;
		   case INPUT_Normal:
            field.index = 2;
			   break;
		   case INPUT_Diffuse:
			   field.index = 3;
			   break;
		   case INPUT_Specular:
			   field.index = 4;
			   break;
		   case INPUT_Tex0:
            field.index = glGetAttribLocation(program, "tex");
			   break;
		   case INPUT_Tex1:
			   field.index = 9;
			   break;
		   case INPUT_Tex2:
			   field.index = 10;
			   break;
		   case INPUT_Tex3:
			   field.index = 11;
			   break;
		   case INPUT_Tex4:
			   field.index = 12;
			   break;
		   case INPUT_Tangent:
			   field.index = 14;
			   break;
		   case INPUT_Binormal:
			   field.index = 15;
			   break;
		   default:
			   break; 
      }
   }
}

} // namespace Graphics
