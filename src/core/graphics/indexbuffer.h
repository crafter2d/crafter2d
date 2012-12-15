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
#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "core/core_base.h"

namespace Graphics
{
   class CORE_API IndexBuffer
   {
   public:
      enum IndexFormat
      {
         eByte,
         eShort,
         eInt,
      };

      IndexBuffer();
      virtual ~IndexBuffer();

    // operations
      virtual bool create(IndexFormat format, int elements, void* data) = 0;
      virtual void release() = 0;

      virtual void enable() const = 0;
      virtual void disable() const = 0;

      virtual void* lock() = 0;
      virtual void unlock() = 0;

   private:
      IndexFormat mFormat;
      int         mElements;
   };
};

#endif // INDEX_BUFFER_H
