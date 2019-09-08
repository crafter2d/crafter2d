
#ifndef MESH_DEFINITION_PROTO_H
#define MESH_DEFINITION_PROTO_H

#include <vector>

#include "core/graphics/animator.h"
#include "core/string/string.h"

#include "componentdefinitionproto.h"

class MeshComponentDefinitionProto : public ComponentDefinitionProto
{
public:
   struct Animation
   {
      c2d::Animator::AnimationType type;
      std::vector<String> frames;
   };
   using Animations = std::vector<Animation>;

   MeshComponentDefinitionProto();
   virtual ~MeshComponentDefinitionProto();

 // data
   int         mWidth;
   int         mHeight;
   float       mAnimationSpeed;
   Animations  mAnimations;
   String      mTexture;

protected:
 // storage
   virtual void virRead(DataStream& stream) override;
   virtual void virWrite(DataStream& stream) const override;
};

#endif // MESH_DEFINITION_PROTO_H
