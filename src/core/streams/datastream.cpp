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
#include "datastream.h"
#ifndef JENGINE_INLINE
#  include "datastream.inl"
#endif

#include "core/string/string.h"

DataStream::DataStream()
{
}

DataStream::DataStream(const DataStream& that)
{
}

DataStream::~DataStream()
{
}

// - Query
int DataStream::getDataSize() const
{
   return 0;
}

const char* DataStream::getData() const
{
   return NULL;
}

// - Operations

void DataStream::copyTo(DataStream& that) const
{
   that.writeBlob(getData(), getDataSize());
}

// - Reading

void DataStream::readInt(int& value)
{
   readBytes(&value, sizeof(int));
}

void DataStream::readUint(unsigned int& value)
{
   readBytes(&value, sizeof(unsigned int));
}

void DataStream::readFloat(float& value)
{
   readBytes(&value, sizeof(float));
}

void DataStream::readBool(bool& value)
{
   value = readByte() != 0;
}

void DataStream::readChar(char& value)
{
   value = readByte();
}

void DataStream::readString(String& value)
{
   int length;
   readInt(length);
   const char* pdata = readBytes(length);
   value.setTo(pdata, length);
}

void DataStream::readBlob(void* pdata, int size)
{

}

// - Writting

void DataStream::writeInt(int value)
{
   writeBytes(&value, sizeof(int));
}

void DataStream::writeUint(unsigned int value)
{
   writeBytes(&value, sizeof(unsigned int));
}

void DataStream::writeFloat(float value)
{
   writeBytes(&value, sizeof(value));
}

void DataStream::writeBool(bool value)
{
   writeBytes(&value, 1);
}

void DataStream::writeChar(char value)
{
   writeBytes(&value, 1);
}

void DataStream::writeString(const String& text)
{
   std::string utf = text.toUtf8();
   writeInt(utf.length());
   writeBytes(utf.c_str(), utf.length());
}

void DataStream::writeBlob(const void* pdata, int size)
{
   writeBytes(pdata, size);
}

// - Overloadable interface

void DataStream::readBytes(void* pbuffer, int amount)
{
   const char* pdata = readBytes(amount);
   memmove(pbuffer, pdata, amount);
}

const char* DataStream::readBytes(int amount)
{
   return NULL;
}

char DataStream::readByte()
{
   return ' ';
}

void DataStream::writeBytes(const void* pbuffer, int amount)
{
}

void DataStream::write(const DataStream& that)
{
   writeInt(that.getDataSize());
   writeBytes(that.getData(), that.getDataSize());
}

void DataStream::read(DataStream& that)
{
   int size = 0;
   readInt(size);
   that.writeBytes(readBytes(size), size);
}

// - Operations
   
void DataStream::clear()
{
}
