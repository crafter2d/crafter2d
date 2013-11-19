
#ifndef MESH_COMPONENT_DEFINITION_H
#define MESH_COMPONENT_DEFINITION_H

#include "componentdefinition.h"

#include "core/graphics/sprites/spritedefinition.h"

class MeshComponentDefinition : public ComponentDefinition
{
public:
   MeshComponentDefinition();

 // get/set
   const Graphics::SpriteDefinition& getSpriteDefinition() const;
         Graphics::SpriteDefinition& getSpriteDefinition();

private:

 // data
   Graphics::SpriteDefinition mSpriteDefinition;
};

#endif // MESH_COMPONENT_DEFINITION_H
