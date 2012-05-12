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
#include "streamwrapper.h"
#ifndef JENGINE_INLINE
#  include "streamwrapper.inl"
#endif

StreamWrapper::StreamWrapper(DataStream& stream):
   mStream(stream)
{
}

// - Query

int StreamWrapper::getDataSize() const
{
   return mStream.getDataSize();
}

const char* StreamWrapper::getData() const
{
   return mStream.getData();
}

// - Overloads

void StreamWrapper::readBytes(void* pbuffer, int amount)
{
   mStream.readBytes(pbuffer, amount);
}

const char* StreamWrapper::readBytes(int amount)
{
   return mStream.readBytes(amount);
}

char StreamWrapper::readByte()
{
   return mStream.readByte();
}

void StreamWrapper::writeBytes(const void* pbuffer, int amount)
{
   mStream.writeBytes(pbuffer, amount);
}
