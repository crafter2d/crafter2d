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
#include "vertexbuffer.h"

#include "core/log/log.h"
#include "core/defines.h"

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
      Log::getInstance().error("VBO Critical Error - out of memory.\n");
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
	for ( int i = 1; i < fvf; i<<=1 )
  {
		switch (fvf & i) {
		case eXYZW:
		case eDiffuse:
		case eSpecular:
			size += sizeof(float) * 4;
			break;
		case eXYZ:
		case eNormal:
		case eTangent:
		case eBinormal:
			size += sizeof(float) * 3;
			break;
		case eXY:
		case eTex0:
		case eTex1:
		case eTex2:
		case eTex3:
		case eTex4:
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
		switch ( fvf & i )
      {
		case eXY:
			field.index = 0;
			field.size = 2;
			break;
		case eXYZ:
			field.index = 0;
			field.size = 3;
			break;
		case eXYZW:
			field.index = 0;
			field.size = 4;
			break;
		case eNormal:
			field.index = 2;
			field.size = 3;
			break;
		case eDiffuse:
			field.index = 3;
			field.size = 4;
			break;
		case eSpecular:
			field.index = 4;
			field.size = 4;
			break;
		case eTex0:
			field.index = 8;
			field.size = 2;
			break;
		case eTex1:
			field.index = 9;
			field.size = 2;
			break;
		case eTex2:
			field.index = 10;
			field.size = 2;
			break;
		case eTex3:
			field.index = 11;
			field.size = 2;
			break;
		case eTex4:
			field.index = 12;
			field.size = 2;
			break;
		case eTangent:
			field.index = 14;
			field.size = 3;
			break;
		case eBinormal:
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
