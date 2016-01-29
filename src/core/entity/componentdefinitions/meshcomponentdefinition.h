
#ifndef MESH_COMPONENT_DEFINITION_H
#define MESH_COMPONENT_DEFINITION_H

#include "componentdefinition.h"

#include "core/graphics/sprites/spritedefinition.h"

class MeshComponentDefinition : public ComponentDefinition
{
public:
   MeshComponentDefinition();

 // get/set
   const c2d::SpriteDefinition& getSpriteDefinition() const {
      return mSpriteDefinition;
   }
   
   c2d::SpriteDefinition& getSpriteDefinition() {
      return mSpriteDefinition;
   }

private:

 // data
   c2d::SpriteDefinition mSpriteDefinition;
};

#endif // MESH_COMPONENT_DEFINITION_H
