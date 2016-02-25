
#include "meshcomponentdefinitionproto.h"

#include "core/entity/components/componentinterface.h"
#include "core/streams/datastream.h"

MeshComponentDefinitionProto::MeshComponentDefinitionProto():
   ComponentDefinitionProto(ComponentInterface::eMeshComponent),
   mWidth(0),
   mHeight(0),
   mAnimationSpeed(0.0f),
   mAnimations()
{
}

MeshComponentDefinitionProto::~MeshComponentDefinitionProto()
{
}

void MeshComponentDefinitionProto::virRead(DataStream& stream)
{
   stream >> mWidth >> mHeight >> mAnimationSpeed;

   uint32_t size = 0;
   stream.readUint32(size);
   mAnimations.resize(size);

   for ( uint32_t index = 0; index < size; ++index )
   {
      uint32_t frameCount;
      Animation& animation = mAnimations[index];
      stream.readUint32((uint32_t&)animation.type);
      stream.readUint32(frameCount);

      for ( uint32_t idx = 0; idx < frameCount; ++idx )
      {
         String name;
         stream >> name;
         animation.frames.push_back(name);
      }
   }
}

void MeshComponentDefinitionProto::virWrite(DataStream& stream) const
{
   stream << mWidth << mHeight << mAnimationSpeed;

   stream.writeUint32(mAnimations.size());
   for ( auto& animation : mAnimations )
   {
      stream.writeUint32(animation.type);
      stream.writeUint32(animation.frames.size());

      for ( auto& frame : animation.frames )
      {
         stream << frame;
      }
   }
}
