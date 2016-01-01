
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
   virtual ~MeshComponentDefinitionProto();

 // data
   int         mWidth;
   int         mHeight;
   String      mTexture;
   float       mAnimationSpeed;
   Animations  mAnimations;

protected:
 // storage
   virtual void virRead(DataStream& stream) override;
   virtual void virWrite(DataStream& stream) const override;
};

#endif // MESH_DEFINITION_PROTO_H
