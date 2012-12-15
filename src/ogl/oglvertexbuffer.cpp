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

#include "core/log/log.h"
#include "core/defines.h"

using namespace Graphics;

#define FLAG_LOCKED		8192

/*!
    \fn VertexBufferObject::VertexBufferObject()
	 \brief Initializes member variables
 */
OGLVertexBuffer::OGLVertexBuffer()
 : VertexBuffer(),
   buffer(0),
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
    \fn VertexBufferObject::create(int length, int usage, int fvf)
 */
bool OGLVertexBuffer::create(int length, int usage, int fvf)
{
   if ( !VertexBuffer::create(length, usage, fvf) )
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

   glGenBuffers(1, &buffer);
   glBindBuffer(GL_ARRAY_BUFFER, buffer);
   glBufferData(GL_ARRAY_BUFFER, mStride * length, 0, flag);

	return true;
}

/*!
    \fn VertexBufferObject::release()
 */
void OGLVertexBuffer::release()
{
	disable();

	glDeleteBuffers(1, &buffer);

	VertexBuffer::release();
}

/*!
    \fn VertexBufferObject::lock(int fvf)
 */
float* OGLVertexBuffer::lock(int fvf)
{
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	float* pointer = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (pointer != NULL)
		locked = true;
	return pointer;
}

/*!
    \fn VertexBufferObject::unlock()
 */
void OGLVertexBuffer::unlock()
{
	if (locked) {
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		locked = false;
	}
}

/*!
    \fn VertexBufferObject::enable()
 */
void OGLVertexBuffer::enable() const
{
   glBindBuffer(GL_ARRAY_BUFFER, buffer);
   for ( int i = mFieldCount - 1; i >= 0; --i )
   {
      const VertexBufferDesc& field = mpFields[i];

		glEnableVertexAttribArray(field.index);
	   glVertexAttribPointer(field.index, field.size, GL_FLOAT, GL_FALSE, mStride, (void*)(field.pos));
   }
}

/*!
    \fn VertexBufferObject::disable()
 */
void OGLVertexBuffer::disable() const
{
   for (int i = 0; i < 16; i++)
   {
	   glDisableVertexAttribArray(i);
   }
}
