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
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <GL/GLee.h>
#include <GL/glu.h>

#include "defines.h"
#include "vertexbufferarray.h"

VertexBufferArray::VertexBufferArray():
   VertexBuffer(),
   mpData(NULL)
{
}


VertexBufferArray::~VertexBufferArray()
{
   release();
}

/*!
    \fn VertexBufferArray::create(VertexBufferDesc* desc)
 */
bool VertexBufferArray::create(const Effect& effect, int length, int usage, int fvf)
{
   if ( !VertexBuffer::create(effect, length, usage, fvf) )
      return false;

	// allocate memory for the buffer
	mpData = new float[mStride * length];
	if ( mpData == NULL )
      return false;

	return true;
}

void VertexBufferArray::release()
{
   if ( mpData != NULL )
   {
      disable();

      delete[] mpData;
      mpData = NULL;
   }

   VertexBuffer::release();
}

float* VertexBufferArray::lock(int fvf)
{
   ASSERT_PTR(mpData);
	return mpData;
}

void VertexBufferArray::unlock()
{
}

void VertexBufferArray::enable()
{
   for ( int i = mFieldCount - 1; i >= 0; --i )
   {
      VertexBufferDesc& field = mFields[i];

      switch ( field.flags )
      {
      case FVF_XY:
	   case FVF_XYZ:
	   case FVF_XYZW:
         glEnableClientState(GL_VERTEX_ARRAY);
         glVertexPointer(field.size, GL_FLOAT, mStride, &mpData[0]);
		   break;
	   case FVF_NORMAL:
         glEnableClientState(GL_NORMAL_ARRAY);
         glNormalPointer(GL_FLOAT, mStride, &mpData[field.pos / sizeof(float)]);
		   break;
	   case FVF_DIFFUSE:
		   break;
	   case FVF_SPECULAR:
		   break;
	   case FVF_TEX0:
         glClientActiveTexture(GL_TEXTURE0);
         glEnableClientState(GL_TEXTURE_COORD_ARRAY);
         glTexCoordPointer(field.size, GL_FLOAT, mStride, &mpData[field.pos / sizeof(float)]);
		   break;
	   case FVF_TEX1:
		   break;
	   case FVF_TEX2:
		   break;
	   case FVF_TEX3:
		   break;
	   case FVF_TEX4:
		   break;
	   case FVF_TANGENT:
		   break;
	   case FVF_BINORMAL:
		   break;
	   default:
		   break;
      }
   }
}

void VertexBufferArray::disable()
{
   for ( int i = 0; i < mFieldCount; ++i )
   {
      VertexBufferDesc& field = mFields[i];

      switch ( field.flags )
      {
      case FVF_XY:
	   case FVF_XYZ:
	   case FVF_XYZW:
         glDisableClientState(GL_VERTEX_ARRAY);
		   break;
	   case FVF_NORMAL:
         glDisableClientState(GL_NORMAL_ARRAY);
		   break;
	   case FVF_DIFFUSE:
		   break;
	   case FVF_SPECULAR:
		   break;
	   case FVF_TEX0:
         glClientActiveTexture(GL_TEXTURE0);
         glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		   break;
	   case FVF_TEX1:
		   break;
	   case FVF_TEX2:
		   break;
	   case FVF_TEX3:
		   break;
	   case FVF_TEX4:
		   break;
	   case FVF_TANGENT:
		   break;
	   case FVF_BINORMAL:
		   break;
	   default:
		   break;
      }
   }
}
