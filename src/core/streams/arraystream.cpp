/***************************************************************************
 *   Copyright (C) 2012 by Jeroen Broekhuizen                              *
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
#include "arraystream.h"

ArrayStream::ArrayStream(const char* pstream, int length):
   DataStream(),
   mpStream(pstream),
   mLength(length),
   mPos(0)
{
}

// - Overloadable Interface

void ArrayStream::reset()
{
   mPos = 0;
}

void ArrayStream::readBytes(void* pbuffer, int amount)
{
   memcpy(pbuffer, mpStream + mPos, amount);
   mPos += amount;
}

char ArrayStream::readByte()
{
   return mpStream[mPos++];
}
