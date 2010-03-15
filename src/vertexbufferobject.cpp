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

#include "vertexbufferobject.h"

#include <GL/GLee.h>

#include "console.h"
#include "opengl.h"
#include "effect.h"

#define FLAG_LOCKED		8192

/*!
    \fn VertexBufferObject::VertexBufferObject()
	 \brief Initializes member variables
 */
VertexBufferObject::VertexBufferObject()
 : VertexBuffer(),
   buffer(0),
   _enableFnc(NULL),
   _disableFnc(NULL)
{
}

/*!
    \fn VertexBufferObject::~VertexBufferObject()
	 \brief Releases member variables
 */
VertexBufferObject::~VertexBufferObject()
{
	release ();
}

/*!
    \fn VertexBufferObject::create(int length, int usage, int fvf)
 */
bool VertexBufferObject::create(const Effect& effect, int length, int usage, int fvf)
{
   if ( !VertexBuffer::create(effect, length, usage, fvf) )
      return false;

	GLuint flag = 0;

	// see what usage is required on this buffer
	if ( IS_SET(usage, eStatic) )
  {
		if ( IS_SET(usage, eWriteOnly) )
			flag = GL_STATIC_DRAW_ARB;
		else if ( IS_SET(usage, eRead) )
			flag = GL_STATIC_READ_ARB;
		else
			flag = GL_STATIC_COPY_ARB;
	}
	else if ( IS_SET(usage, eDynamic) )
  {
		if ( IS_SET(usage, eWriteOnly) )
			flag = GL_DYNAMIC_DRAW_ARB;
		else if ( IS_SET(usage, eRead) )
			flag = GL_DYNAMIC_READ_ARB;
		else
			flag = GL_DYNAMIC_COPY_ARB;
	}
	else if ( IS_SET(usage, eStream) )
  {
		if ( IS_SET(usage, eWriteOnly) )
			flag = GL_STREAM_DRAW_ARB;
		else if ( IS_SET(usage, eRead) )
			flag = GL_STREAM_READ_ARB;
		else
			flag = GL_STREAM_COPY_ARB;
	}
	else 
  {
      Console::getInstance().print("VBO Error - Invalid usage flag.\n");
      return false;
   }

   glGenBuffersARB (1, &buffer);
   glBindBufferARB (GL_ARRAY_BUFFER_ARB, buffer);
   glBufferDataARB (GL_ARRAY_BUFFER_ARB, mStride * length, 0, flag);

   if ( effect.hasPath() )
   {
      _enableFnc  = &VertexBufferObject::enableBuffers;
      _disableFnc = &VertexBufferObject::disableBuffers;
   }
   else
   {
      _enableFnc  = &VertexBufferObject::enableArrays;
      _disableFnc = &VertexBufferObject::disableArrays;
   }

	return true;
}

/*!
    \fn VertexBufferObject::release()
 */
void VertexBufferObject::release()
{
	// destroy the gl VBO
	glDeleteBuffersARB (1, &buffer);

	VertexBuffer::release();
}

/*!
    \fn VertexBufferObject::lock(int fvf)
 */
float* VertexBufferObject::lock(int fvf)
{
	glBindBufferARB (GL_ARRAY_BUFFER_ARB, buffer);
	float* pointer = (float*)glMapBufferARB (GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY_ARB);
	if (pointer != NULL)
		locked = true;
	return pointer;
}

/*!
    \fn VertexBufferObject::unlock()
 */
void VertexBufferObject::unlock()
{
	if (locked) {
		glBindBufferARB (GL_ARRAY_BUFFER_ARB, buffer);
		glUnmapBufferARB (GL_ARRAY_BUFFER_ARB);
		locked = false;
	}
}

/*!
    \fn VertexBufferObject::enable()
 */
void VertexBufferObject::enable()
{
   (this->*_enableFnc)();
}

void VertexBufferObject::enableBuffers()
{
   glBindBufferARB (GL_ARRAY_BUFFER_ARB, buffer);
   for ( int i = mFieldCount - 1; i >= 0; --i )
   {
      VertexBufferDesc& field = mFields[i];

		glEnableVertexAttribArrayARB (field.index);
	   glVertexAttribPointerARB (field.index, field.size, GL_FLOAT, GL_FALSE, mStride, (void*)(field.pos));
   }
}

void VertexBufferObject::enableArrays()
{
   glBindBuffer(GL_ARRAY_BUFFER, buffer);
   for ( int i = mFieldCount- 1; i >= 0; --i )
   {
      VertexBufferDesc& field = mFields[i];

      switch ( field.flags )
      {
      case eXY:
	   case eXYZ:
	   case eXYZW:
         glEnableClientState(GL_VERTEX_ARRAY);
         glVertexPointer(field.size, GL_FLOAT, mStride, 0);
		   break;
	   case eNormal:
         glEnableClientState(GL_NORMAL_ARRAY);
         glNormalPointer(GL_FLOAT, mStride, (void*)(field.pos));
		   break;
	   case eDiffuse:
		   break;
	   case eSpecular:
		   break;
	   case eTex0:
         glClientActiveTexture(GL_TEXTURE0);
         glEnableClientState(GL_TEXTURE_COORD_ARRAY);
         glTexCoordPointer(field.size, GL_FLOAT, mStride, (void*)(field.pos));
		   break;
	   case eTex1:
		   break;
	   case eTex2:
		   break;
	   case eTex3:
		   break;
	   case eTex4:
		   break;
	   case eTangent:
		   break;
	   case eBinormal:
		   break;
	   default:
		   break;
      }
   }
}

/*!
    \fn VertexBufferObject::disable()
 */
void VertexBufferObject::disable()
{
   (this->*_disableFnc)();
}

void VertexBufferObject::disableBuffers()
{
   for (int i = 0; i < 16; i++)
   {
	   glDisableVertexAttribArrayARB (i);
   }
}

void VertexBufferObject::disableArrays()
{
   for ( int i = 0; i < mFieldCount; ++i )
   {
      VertexBufferDesc& field = mFields[i];

      switch ( field.flags )
      {
      case eXY:
	   case eXYZ:
	   case eXYZW:
         glDisableClientState(GL_VERTEX_ARRAY);
		   break;
	   case eNormal:
         glDisableClientState(GL_NORMAL_ARRAY);
		   break;
	   case eDiffuse:
		   break;
	   case eSpecular:
		   break;
	   case eTex0:
         glClientActiveTexture(GL_TEXTURE0);
         glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		   break;
	   case eTex1:
		   break;
	   case eTex2:
		   break;
	   case eTex3:
		   break;
	   case eTex4:
		   break;
	   case eTangent:
		   break;
	   case eBinormal:
		   break;
	   default:
		   break;
      }
   }
}
