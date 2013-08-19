
#ifndef MESH_COMPONENT_DEFINITION_H
#define MESH_COMPONENT_DEFINITION_H

#include "componentdefinition.h"

#include "engine/sprites/spritedefinition.h"

class MeshComponentDefinition : public ComponentDefinition
{
public:
   MeshComponentDefinition();

 // get/set
   const SpriteDefinition& getSpriteDefinition() const;
         SpriteDefinition& getSpriteDefinition();

private:

 // data
   SpriteDefinition mSpriteDefinition;
};

#endif // MESH_COMPONENT_DEFINITION_H
