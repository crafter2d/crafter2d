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
#ifndef _BITSTREAM_H_
#define _BITSTREAM_H_

#define MAX_BITSTREAM_BUFSIZE 1500

#include <string>

#include "core/defines.h"

class NetObject;
class Vector;

/// @author Jeroen Broekhuizen
/// 
/// The BitStream class is used for packing and unpacking class information
/// in a small datastructure for transmission of the internet or to save objects
/// in databases.
class BitStream
{
public:
               BitStream();
               BitStream(int reserve);
               BitStream(const BitStream& that);
               ~BitStream();

   void        clear();
   bool        empty();
   void        reset();

   void        setBuffer(const char* data, int size);
   int         getSize() const;
   const char* getBuf() const;
   int         getLen() const;
   bool        end() const;

   std::string readString();
   int         readInt();
   bool        readBool();
   void        readRaw(char* data, int amount);

   void        writeString(const std::string& s);
   void        writeInt(int i);
   void        writeBool(bool b);
   void        writeRaw(const char* data, int size);

   BitStream&  operator<<(const Vector& v);
   BitStream&  operator<<(float f);
   BitStream&  operator<<(int i);
   BitStream&  operator<<(uint i);
   BitStream&  operator<<(bool b);
   BitStream&  operator<<(char c);
   BitStream&  operator<<(const std::string& text);
   BitStream&  operator<<(const BitStream* stream);

   BitStream&  operator>>(Vector& v);
   BitStream&  operator>>(float& f);
   BitStream&  operator>>(int& i);
   BitStream&  operator>>(uint& i);
   BitStream&  operator>>(bool& b);
   BitStream&  operator>>(char& c);
   BitStream&  operator>>(std::string& str);
   BitStream&  operator>>(BitStream& stream);

protected:
   void ensureFits(int size);

   char* buf;
   int   pos;
   int   size;
   int   bufsize;
};

#ifdef JENGINE_INLINE
#  include "bitstream.inl"
#endif

#endif
