
#ifndef ENTITY_DEFINITION_H
#define ENTITY_DEFINITION_H

#include <vector>

#include "core/string/string.h"

#include "engine/components/componentdefinition.h"

class EntityDefinition
{
public:
 // types
   typedef std::vector<ComponentDefinition*> CompDefs;

   EntityDefinition();

 // get/set
   const CompDefs& getComponentDefinitions() const;

   const String& getClassName() const;
   void          setClassName(const String& name);

 // registration
   void addComponentDefinition(ComponentDefinition* pdefinition);

private:

 // data
   CompDefs mDefinitions;
   String   mClassName;
};

#endif // ENTITY_DEFINITION_H
