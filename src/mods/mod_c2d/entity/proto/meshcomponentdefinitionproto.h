
#ifndef MESH_DEFINITION_PROTO_H
#define MESH_DEFINITION_PROTO_H

#include <vector>

#include "core/string/string.h"

#include "componentdefinitionproto.h"

class MeshComponentDefinitionProto : public ComponentDefinitionProto
{
public:
   typedef std::vector<int> Animations;

   MeshComponentDefinitionProto();

   virtual void read(DataStream& stream) override;
   virtual void write(DataStream& stream) const override;

   int         mWidth;
   int         mHeight;
   String      mTexture;
   float       mAnimationSpeed;
   Animations  mAnimations;
};

#endif // MESH_DEFINITION_PROTO_H
