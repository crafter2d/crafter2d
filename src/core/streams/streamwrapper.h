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
#ifndef STREAM_WRAPPED_H_
#define STREAM_WRAPPED_H_

#include "core/core_base.h"

#include "datastream.h"

class CORE_API StreamWrapper : public DataStream
{
public:
   StreamWrapper(DataStream& stream);

 // query
   virtual int         getDataSize() const;
   virtual const char* getData() const;

protected:
 // get/set
   DataStream& getStream();

 // overloads
   virtual       void  readBytes(void* pbuffer, int amount);
   virtual const char* readBytes(int amount);
   virtual       char  readByte();

   virtual       void  writeBytes(const void* pbuffer, int amount);

private:
   DataStream& mStream;
};

#ifdef JENGINE_INLINE
#  include "streamwrapper.inl"
#endif

#endif // STREAM_WRAPPED_H_
