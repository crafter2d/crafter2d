/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "object.h"
#include "vertexbufferobject.h"
#include <vector>

class Object3D : public Object
{
   struct Face {
      int idx[3];
   };
   struct Mesh {
      Vector3* verts;
      Uint32 nverts;
      Face* faces;
      Uint32 nfaces;
   };

public:
   Object3D(void);
   ~Object3D(void);

   bool create(const char* filename=NULL);
   
   virtual void pack(BitStream& stream) const;

protected:
   VertexBufferObject vbo;
   std::vector<Mesh*> meshes;
};

#endif
