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
#include "object3d.h"

#include <fstream>

#include "math/vector3.h"

struct Face {
   int idx[3];
};

struct Mesh {
   Vector3* verts;
   Uint32 nverts;
   Face* faces;
   Uint32 nfaces;
};

Object3D::Object3D()
{
}

Object3D::~Object3D()
{
}

bool Object3D::create(const char* filename)
{
   std::ifstream infile;
   infile.open(filename, std::ifstream::binary);
   if (!infile.good())
      return false;

   char header[7]="";
   infile.read(header, 6);
   if (strcmp(header, "JSSE3D") != 0) {
      return false;
   }

   int nmeshes = 0;
   infile.read ((char*)&nmeshes, sizeof(int));
   meshes.resize(nmeshes);

   // load the meshes into memory
   for (int i=0; i < nmeshes; ++i)
   {
      Mesh* mesh = new Mesh();
      infile.read ((char*)&mesh->nverts, sizeof(int));
      infile.read ((char*)&mesh->nfaces, sizeof(int));

      mesh->verts = new Vector3[mesh->nverts];
      mesh->faces = new Face[mesh->nfaces];

      // read in the vertices
      float vert[3];
      for (int v=0; v<mesh->nverts; ++v)
      {
         infile.read ((char*)vert, sizeof(float)*3);
         mesh->verts[v].set(vert[0], vert[1], vert[2]);
      }

      // read in the faces
      for (int f=0; f<mesh->nfaces; ++f)
         infile.read ((char*)mesh->faces[f].idx, sizeof(int)*3);

      meshes[i] = mesh;
   }
   infile.close();
   return true;
}

void Object3D::pack(BitStream& stream) const
{
}
