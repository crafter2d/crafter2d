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
#ifndef DATA_STREAM_H
#define DATA_STREAM_H

#include <stdint.h>

#include "core/string/char.h"
#include "core/core_base.h"

class String;

class CORE_API DataStream
{
public:
            DataStream();
   virtual ~DataStream() = 0;

 // query
   virtual int         getDataSize() const = 0;
   virtual const char* getData() const = 0;

 // operators
   DataStream&  operator<<(bool b);
   DataStream&  operator<<(int i);
   DataStream&  operator<<(float f);
   DataStream&  operator<<(double d);
   DataStream&  operator<<(UChar c);
   DataStream&  operator<<(const String& text);

   DataStream&  operator>>(bool& b);
   DataStream&  operator>>(int& i);
   DataStream&  operator>>(float& f);
   DataStream&  operator>>(double& d);
   DataStream&  operator>>(UChar& c);
   DataStream&  operator>>(String& str);

 // operations
   void copyTo(DataStream& that) const;

 // overloadables
   virtual void writeInt(int32_t value);
   virtual void writeUint(uint32_t value);
   virtual void writeUint8(uint8_t value);
   virtual void writeUint16(uint16_t value);
   virtual void writeUint32(uint32_t value);
   virtual void writeUint64(uint64_t value);
   virtual void writeFloat(float value);
   virtual void writeDouble(double value);
   virtual void writeBool(bool value);
   virtual void writeChar(UChar c);
   virtual void writeString(const String& text);
           void writeBlob(const void* pdata, int size);
   
   virtual void readInt(int32_t& value);
   virtual void readUint(uint32_t& value);
   virtual void readUint8(uint8_t& value);
   virtual void readUint16(uint16_t& value);
   virtual void readUint32(uint32_t& value);
   virtual void readUint64(uint64_t& value);
   virtual void readFloat(float& value);
   virtual void readDouble(double& value);
   virtual void readBool(bool& value);
   virtual void readChar(UChar& value);
   virtual void readString(String& value);
           void readBlob(void* pdata, int size);

   void write(const DataStream& that);
   void read(DataStream& that);

 // operations
   virtual void clear();

protected:
   friend class StreamWrapper;

   virtual       void  readBytes(void* pbuffer, int amount);
   virtual const char* readBytes(int amount);
   virtual       char  readByte();

   virtual       void  writeBytes(const void* pbuffer, int amount);
};

#ifdef JENGINE_INLINE
#  include "datastream.inl"
#endif

#endif // DATA_STREAM_H
