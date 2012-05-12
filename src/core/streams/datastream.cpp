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

// - Reading

void DataStream::read(int& value)
{
   readBytes(&value, sizeof(int));
}

void DataStream::read(unsigned int& value)
{
   readBytes(&value, sizeof(unsigned int));
}

void DataStream::read(float& value)
{
   readBytes(&value, sizeof(float));
}

void DataStream::read(bool& value)
{
   value = static_cast<bool>(readByte());
}

void DataStream::read(char& value)
{
   value = readByte();
}

void DataStream::read(std::string& value)
{
   int length;
   read(length);
   const char* pdata = readBytes(length);
   value.assign(pdata, length);
}

// - Writting

void DataStream::write(int value)
{
   writeBytes(&value, sizeof(int));
}

void DataStream::write(unsigned int value)
{
   writeBytes(&value, sizeof(unsigned int));
}

void DataStream::write(float value)
{
   writeBytes(&value, sizeof(value));
}

void DataStream::write(bool value)
{
   writeBytes(&value, 1);
}

void DataStream::write(char value)
{
   writeBytes(&value, 1);
}

void DataStream::write(const std::string& text)
{
   write(text.length());
   writeBytes(text.data(), text.length());
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
