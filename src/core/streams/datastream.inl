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

INLINE DataStream& DataStream::operator<<(float value)
{
   write(value);
   return *this;
}

INLINE DataStream& DataStream::operator<<(int value)
{
   write(value);
   return *this;
}

INLINE DataStream& DataStream::operator<<(unsigned int value)
{
   write(value);
   return *this;
}

INLINE DataStream& DataStream::operator<<(bool value)
{
   write(value);
   return *this;
}

INLINE DataStream& DataStream::operator<<(char value)
{
   write(value);
   return *this;
}

INLINE DataStream& DataStream::operator<<(const std::string& value)
{
   write(value);
   return *this;
}

INLINE DataStream& DataStream::operator>>(float& value)
{
   read(value);
   return *this;
}

INLINE DataStream& DataStream::operator>>(int& value)
{
   read(value);
   return *this;
}

INLINE DataStream& DataStream::operator>>(unsigned int& value)
{
   read(value);
   return *this;
}

INLINE DataStream& DataStream::operator>>(bool& value)
{
   read(value);
   return *this;
}

INLINE DataStream& DataStream::operator>>(char& value)
{
   read(value);
   return *this;
}

INLINE DataStream& DataStream::operator>>(std::string& value)
{
   read(value);
   return *this;
}
