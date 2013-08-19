
#include "meshcomponentdefinition.h"

#include "core/defines.h"

MeshComponentDefinition::MeshComponentDefinition():
   ComponentDefinition(ComponentInterface::eMeshComponent),
   mSpriteDefinition()
{
}

// - Get/set

// get/set
   
const SpriteDefinition& MeshComponentDefinition::getSpriteDefinition() const
{
   return mSpriteDefinition;
}
         
SpriteDefinition& MeshComponentDefinition::getSpriteDefinition()
{
   return mSpriteDefinition;
}
