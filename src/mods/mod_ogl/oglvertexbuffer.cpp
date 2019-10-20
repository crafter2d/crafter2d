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

#include "GL/gl3w.h"
#include <GL/glu.h>

#include "core/graphics/vertexlayout.h"
#include "core/graphics/vertexlayoutelement.h"
#include "core/log/log.h"
#include "core/defines.h"

namespace Graphics
{

#define FLAG_LOCKED		   8192
#define BUFFER_OFFSET(i)   ((char *)nullptr + (i))


/*!
    \fn VertexBufferObject::VertexBufferObject()
	 \brief Initializes member variables
 */
OGLVertexBuffer::OGLVertexBuffer()
 : VertexBuffer(),
   mVAO(0),
   mBuffer(0),
   locked(false),
   mLock(0)
{
}

/*!
    \fn VertexBufferObject::~VertexBufferObject()
	 \brief Releases member variables
 */
OGLVertexBuffer::~OGLVertexBuffer()
{
	release();
}

/*!
    \fn VertexBufferObject::create(int length, int usage)
 */
bool OGLVertexBuffer::create(const VertexLayout& layout, int length, int usage)
{
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

   glGenVertexArrays(1, &mVAO);
   glBindVertexArray(mVAO);

   glGenBuffers(1, &mBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
   glBufferData(GL_ARRAY_BUFFER, layout.getStride() * length, 0, flag);

   for ( const auto& field : layout )
   {
      GLint size = field.type < 5 ? field.type : 1;
      GLenum type = field.type < 5 ? GL_FLOAT : GL_UNSIGNED_INT;

      glEnableVertexAttribArray(field.index);
      glVertexAttribPointer(field.index, size, type, GL_FALSE, layout.getStride(), BUFFER_OFFSET(field.pos));
   }

   glBindVertexArray(0);

	return true;
}

/*!
    \fn VertexBufferObject::release()
 */
void OGLVertexBuffer::release()
{
   glBindVertexArray(mVAO);
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
void* OGLVertexBuffer::lock(RenderContext& context)
{
   C2D_UNUSED(context);

   GLenum err = glGetError();
   if ( err != GL_NO_ERROR )
   {
      int aap = 3;
   }
   
   glBindVertexArray(mVAO);
   glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
	void* pointer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
   if ( pointer != nullptr ) {
      locked = true;
      mLock++;
   }

	return pointer;
}

/*!
    \fn VertexBufferObject::unlock()
 */
void OGLVertexBuffer::unlock(RenderContext& context)
{
   C2D_UNUSED(context);

   ASSERT(mLock > 0);
   
	if (locked) {
      glBindVertexArray(mVAO);
      glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
      glUnmapBuffer(GL_ARRAY_BUFFER);
      locked = false;
      mLock--;
   }
}

void OGLVertexBuffer::update(RenderContext& context, void* pdata, int count)
{
   C2D_UNUSED(context);
   C2D_UNUSED(pdata);
   C2D_UNUSED(count);
   // not yet implemented
}

/*!
    \fn VertexBufferObject::enable()
 */
void OGLVertexBuffer::enable(RenderContext& context) const
{
   C2D_UNUSED(context);
   
   glBindVertexArray(mVAO);
}

/*!
    \fn VertexBufferObject::disable()
 */
void OGLVertexBuffer::disable(RenderContext& context) const
{
   C2D_UNUSED(context);
   
   glBindVertexArray(0);
}

} // namespace Graphics
