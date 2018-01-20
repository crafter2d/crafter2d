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

#include <cstring>

#include "core/string/string.h"
#include "core/defines.h"

DataStream::DataStream()
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

void DataStream::readBool(bool& value)
{
   value = readByte() != 0;
}

void DataStream::readInt(int32_t& value)
{
   readBytes(&value, sizeof(int32_t));
}

void DataStream::readUint8(uint8_t& value)
{
   value = static_cast<uint8_t>(readByte());
}

void DataStream::readUint16(uint16_t& value)
{
   readBytes(&value, sizeof(uint16_t));
}

void DataStream::readUint32(uint32_t& value)
{
   readBytes(&value, sizeof(uint32_t));
}

void DataStream::readUint64(uint64_t& value)
{
   readBytes(&value, sizeof(uint64_t));
}

void DataStream::readFloat(float& value)
{
   readBytes(&value, sizeof(float));
}

void DataStream::readDouble(double& value)
{
   readBytes(&value, sizeof(double));
}

void DataStream::readChar(UChar& value)
{
   value = readByte();
}

void DataStream::readString(String& value)
{
   int length;
   readInt(length);
   const char* pdata = readBytes(length);
   value.setToUtf8(pdata, length);
}

void DataStream::readBlob(void* pdata, int size)
{
   const char* ptr = readBytes(size);
   memcpy(pdata, ptr, size);
}

const char* DataStream::readBlob(int size)
{
   return readBytes(size);
}

// - Writting

void DataStream::writeBool(bool value)
{
   writeBytes(&value, 1);
}

void DataStream::writeInt(int32_t value)
{
   writeBytes(&value, sizeof(int32_t));
}

void DataStream::writeUint8(uint8_t value)
{
   writeBytes(&value, sizeof(uint8_t));
}

void DataStream::writeUint16(uint16_t value)
{
   writeBytes(&value, sizeof(uint16_t));
}

void DataStream::writeUint32(uint32_t value)
{
   writeBytes(&value, sizeof(uint32_t));
}

void DataStream::writeUint64(uint64_t value)
{
   writeBytes(&value, sizeof(uint64_t));
}

void DataStream::writeFloat(float value)
{
   writeBytes(&value, sizeof(float));
}

void DataStream::writeDouble(double value)
{
   writeBytes(&value, sizeof(double));
}

void DataStream::writeChar(UChar value)
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
   C2D_UNUSED(amount);
   return NULL;
}

char DataStream::readByte()
{
   return ' ';
}

void DataStream::writeBytes(const void* pbuffer, int amount)
{
   C2D_UNUSED(pbuffer);
   C2D_UNUSED(amount);
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
