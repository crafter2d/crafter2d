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
      case eXY:
	   case eXYZ:
	   case eXYZW:
         glEnableClientState(GL_VERTEX_ARRAY);
         glVertexPointer(field.size, GL_FLOAT, mStride, &mpData[0]);
		   break;
	   case eNormal:
         glEnableClientState(GL_NORMAL_ARRAY);
         glNormalPointer(GL_FLOAT, mStride, &mpData[field.pos / sizeof(float)]);
		   break;
	   case eDiffuse:
		   break;
	   case eSpecular:
		   break;
	   case eTex0:
         glClientActiveTexture(GL_TEXTURE0);
         glEnableClientState(GL_TEXTURE_COORD_ARRAY);
         glTexCoordPointer(field.size, GL_FLOAT, mStride, &mpData[field.pos / sizeof(float)]);
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

void VertexBufferArray::disable()
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
