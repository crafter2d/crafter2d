
#include "meshcomponentdefinition.h"

#include "core/defines.h"

MeshComponentDefinition::MeshComponentDefinition():
   ComponentDefinition(ComponentInterface::eMeshComponent),
   mSpriteDefinition()
{
}

// - Get/set

// get/set
   
const Graphics::SpriteDefinition& MeshComponentDefinition::getSpriteDefinition() const
{
   return mSpriteDefinition;
}
         
Graphics::SpriteDefinition& MeshComponentDefinition::getSpriteDefinition()
{
   return mSpriteDefinition;
}
