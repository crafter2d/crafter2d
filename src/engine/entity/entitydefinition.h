
#ifndef ENTITY_DEFINITION_H
#define ENTITY_DEFINITION_H

#include <vector>

#include "engine/components/componentdefinition.h"

class EntityDefinition
{
public:
   EntityDefinition();

 // registration
   void addComponentDefinition(ComponentDefinition* pdefinition);

private:
 // types
   typedef std::vector<ComponentDefinition*> CompDefs;

 // data
   CompDefs mDefinitions;
};

#endif // ENTITY_DEFINITION_H
