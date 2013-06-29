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

using namespace Graphics;

VertexBuffer::VertexBuffer(VertexInputLayout& layout):
   mLayout(layout)
{
}

VertexBuffer::~VertexBuffer()
{
}

bool VertexBuffer::create(int length, int usage)
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

	return true;
}

void VertexBuffer::release()
{
}
