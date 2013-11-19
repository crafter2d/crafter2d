
#include "meshcomponentdefinitionproto.h"

#include "core/entity/components/componentinterface.h"
#include "core/streams/datastream.h"

MeshComponentDefinitionProto::MeshComponentDefinitionProto():
   ComponentDefinitionProto(ComponentInterface::eMeshComponent)
{
}

void MeshComponentDefinitionProto::read(DataStream& stream)
{
   stream >> mWidth >> mHeight >> mTexture >> mAnimationSpeed;

   uint32_t size = 0;
   stream.readUint(size);
   for ( uint32_t index = 0; index < size; ++index )
   {
      int length;
      stream >> length;
      mAnimations.push_back(length);
   }
}

void MeshComponentDefinitionProto::write(DataStream& stream) const
{
   stream.writeInt(ComponentInterface::eMeshComponent);
   stream << mWidth << mHeight << mTexture << mAnimationSpeed;

   stream.writeUint(mAnimations.size());
   for ( std::size_t index = 0; index < mAnimations.size(); ++index )
   {
      stream << mAnimations[index];
   }
}
