
#ifndef MESH_COMPONENT_DEFINITION_H
#define MESH_COMPONENT_DEFINITION_H

#include "core/string/string.h"

#include "componentdefinition.h"

class Animator;

class MeshComponentDefinition : public ComponentDefinition
{
public:

 // overrides
   virtual void load(const TiXmlElement& element) override;

   virtual Component* instantiate() const;

private:

 // data
   String      mTexture;
   Animator*   mpAnimator;
   int         mWidth;
   int         mHeight;
};

#endif // MESH_COMPONENT_DEFINITION_H
