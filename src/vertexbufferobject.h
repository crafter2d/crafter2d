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
#ifndef CVERTEXBUFFEROBJECT_H
#define CVERTEXBUFFEROBJECT_H

#include "vertexbuffer.h"
#include "codepath.h"

/**
@author Jeroen Broekhuizen
*/
class VertexBufferObject : public VertexBuffer
{
public:
   typedef void (VertexBufferObject::*PFunction)();

    VertexBufferObject();
    virtual ~VertexBufferObject();

    bool create(const Effect& effect, int length, int usage, int fvf);
	 void release();

    float* lock(int fvf);
    void unlock();
    void enable();
    void disable();

private:
   void enableBuffers();
   void disableBuffers();

   void enableArrays();
   void disableArrays();

   unsigned int buffer;
   bool locked;

   PFunction _enableFnc;
   PFunction _disableFnc;
};

#endif
