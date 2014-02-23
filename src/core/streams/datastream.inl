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
#include "core/defines.h"

// - Operators

INLINE DataStream& DataStream::operator<<(bool value)
{
   writeBool(value);
   return *this;
}

INLINE DataStream& DataStream::operator<<(int value)
{
   writeInt(value);
   return *this;
}

INLINE DataStream& DataStream::operator<<(float value)
{
   writeFloat(value);
   return *this;
}

INLINE DataStream& DataStream::operator<<(double value)
{
   writeDouble(value);
   return *this;
}

INLINE DataStream& DataStream::operator<<(UChar value)
{
   writeChar(value);
   return *this;
}

INLINE DataStream& DataStream::operator<<(const String& value)
{
   writeString(value);
   return *this;
}

INLINE DataStream& DataStream::operator>>(bool& value)
{
   readBool(value);
   return *this;
}

INLINE DataStream& DataStream::operator>>(int& value)
{
   readInt(value);
   return *this;
}

INLINE DataStream& DataStream::operator>>(float& value)
{
   readFloat(value);
   return *this;
}

INLINE DataStream& DataStream::operator>>(double& value)
{
   readDouble(value);
   return *this;
}

INLINE DataStream& DataStream::operator>>(UChar& value)
{
   readChar(value);
   return *this;
}

INLINE DataStream& DataStream::operator>>(String& value)
{
   readString(value);
   return *this;
}
