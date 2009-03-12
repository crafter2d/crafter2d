
#include <fstream>
#include "object3d.h"

Object3D::Object3D(void)
{
}

Object3D::~Object3D(void)
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
   for (int i=0; i < nmeshes; ++i) {
      Mesh* mesh = new Mesh();
      infile.read ((char*)&mesh->nverts, sizeof(int));
      infile.read ((char*)&mesh->nfaces, sizeof(int));

      mesh->verts = new Vector3[mesh->nverts];
      mesh->faces = new Face[mesh->nfaces];

      // read in the vertices
      for (int v=0; v<mesh->nverts; ++v)
         infile.read ((char*)&mesh->verts[v].x, sizeof(float)*3);
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
