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

#include "console.h"
#include "defines.h"
#include "vertexbuffer.h"

VertexBuffer::VertexBuffer()
{
}

VertexBuffer::~VertexBuffer()
{
}

bool VertexBuffer::create(const Effect& effect, int length, int usage, int fvf)
{
   /* GameDev:
	 * So it is possible to put all stuff in one buffer and render that at the same time, see below!:
	 *
	 * glVertexPointer(3, GL_FLOAT, 4*sizeof(unsigned char) + 3*sizeof(float), 0);
	 * glColorPointer(4, GL_UNSIGNED_BYTE, 4*sizeof(unsigned char) + 3*sizeof(float), (void*)(3*sizeof(float)));

	 * The stride isn't the distance from the end of one attribute to the beginning of the next
	 * attribute (of that type), but rather, the distance from the beginning of one to the beginning
	 * of the next. And you can't use 0 for the color pointer offset because the color data doesn't start at 0.
	 */

   // determine size of the vertex structure
	mFieldCount = getFieldCount (fvf);
	mStride     = getFVFSize (fvf);

	// allocate memory & fill field descriptions
	mFields = new VertexBufferDesc[mFieldCount];
	if ( mFields == NULL )
	{
      Console::getInstance().print("VBO Critical Error - out of memory.\n");
		return false;
	}

	fillDescription(fvf);

	return true;
}

void VertexBuffer::release()
{
   if ( mFields != NULL )
   {
	   delete[] mFields;
      mFields = 0;
   }
}

/*!
    \fn VertexBuffer::getFVFSize(int fvf)
	 \brief Calculates the complete size in bytes of all the vertex attribute in the flag
	 \param[in] fvf he vertex attribute flag which was used to create the vertex buffer
	 \return The total size in bytes of all vertex attributes
 */
int VertexBuffer::getFVFSize(int fvf)
{
	int size = 0;
	for (int i = 1; i < fvf; i<<=1) {
		switch (fvf & i) {
		case FVF_XYZW:
		case FVF_DIFFUSE:
		case FVF_SPECULAR:
			size += sizeof(float) * 4;
			break;
		case FVF_XYZ:
		case FVF_NORMAL:
		case FVF_TANGENT:
		case FVF_BINORMAL:
			size += sizeof(float) * 3;
			break;
		case FVF_XY:
		case FVF_TEX0:
		case FVF_TEX1:
		case FVF_TEX2:
		case FVF_TEX3:
		case FVF_TEX4:
			size += sizeof(float) * 2;
			break;
		default:
			break;
		}
	}
	return size;
}

/*!
    \fn VertexBuffer::getFieldCount(int fvf)
	 \brief Determines the number of vertex attributes in the attribute parameter
	 \param[in] fvf the vertex attribute flag which was used to create the vertex buffer
	 \return The number of attributes included in the flag
 */
int VertexBuffer::getFieldCount(int fvf)
{
	int count = 0;
	for (int i = 1; i < fvf; i <<= 1) {
		if (fvf & i)
			count++;
	}
	return count;
}


/*!
    \fn VertexBuffer::fillDescription (int fvf)
	 \brief Fills in the buffer descriptor with the required information.
	 \param[in] fvf the vertex attributes that where used to create the vertex buffer
	 \param[in] descs array of VertexBufferDesc structures
	 \return Nothing
 */
void VertexBuffer::fillDescription (int fvf)
{
	int pos = 0;
	int cnt = 0, i = 1;

	while (i <= fvf)
	{
		if (!(i & fvf))
		{
			i <<= 1;
			continue;
		}

		VertexBufferDesc& field = mFields[cnt];

		field.pos = pos;
		field.flags = i;
		switch (fvf & i) {
		case FVF_XY:
			field.index = 0;
			field.size = 2;
			break;
		case FVF_XYZ:
			field.index = 0;
			field.size = 3;
			break;
		case FVF_XYZW:
			field.index = 0;
			field.size = 4;
			break;
		case FVF_NORMAL:
			field.index = 2;
			field.size = 3;
			break;
		case FVF_DIFFUSE:
			field.index = 3;
			field.size = 4;
			break;
		case FVF_SPECULAR:
			field.index = 4;
			field.size = 4;
			break;
		case FVF_TEX0:
			field.index = 8;
			field.size = 2;
			break;
		case FVF_TEX1:
			field.index = 9;
			field.size = 2;
			break;
		case FVF_TEX2:
			field.index = 10;
			field.size = 2;
			break;
		case FVF_TEX3:
			field.index = 11;
			field.size = 2;
			break;
		case FVF_TEX4:
			field.index = 12;
			field.size = 2;
			break;
		case FVF_TANGENT:
			field.index = 14;
			field.size = 3;
			break;
		case FVF_BINORMAL:
			field.index = 15;
			field.size = 3;
			break;
		default:
			break;
		}

		pos += field.size * sizeof(float);
		i <<= 1;
		cnt++;
	}
}
