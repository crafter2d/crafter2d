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

#include "core/core_base.h"

#include <string>

class CORE_API DataStream
{
public:
            DataStream();
   virtual ~DataStream() = 0;

 // query
   virtual int         getDataSize() const = 0;
   virtual const char* getData() const = 0;

 // operators
   DataStream&  operator<<(float f);
   DataStream&  operator<<(int i);
   DataStream&  operator<<(unsigned int i);
   DataStream&  operator<<(bool b);
   DataStream&  operator<<(char c);
   DataStream&  operator<<(const std::string& text);

   DataStream&  operator>>(float& f);
   DataStream&  operator>>(int& i);
   DataStream&  operator>>(unsigned int& i);
   DataStream&  operator>>(bool& b);
   DataStream&  operator>>(char& c);
   DataStream&  operator>>(std::string& str);

 // overloadables
   virtual void write(int value);
   virtual void write(unsigned int value);
   virtual void write(float value);
   virtual void write(bool value);
   virtual void write(char c);
   virtual void write(const std::string& text);
   
   virtual void read(int& value);
   virtual void read(unsigned int& value);
   virtual void read(float& value);
   virtual void read(bool& value);
   virtual void read(char& value);
   virtual void read(std::string& value);

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
