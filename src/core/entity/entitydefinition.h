
#ifndef ENTITY_DEFINITION_H
#define ENTITY_DEFINITION_H

#include <vector>

#include "core/entity/componentdefinitions/componentdefinition.h"
#include "core/string/string.h"
#include "core/math/vector.h"

class EntityLinkDefinition;

class EntityDefinition
{
public:
 // types
   typedef std::vector<EntityDefinition*> Children;
   typedef std::vector<EntityLinkDefinition*> Links;
   typedef std::vector<ComponentDefinition*> Components;

   EntityDefinition();

 // get/set
   const Children&   getChildren() const;
   const Links&      getLinks() const;
   const Components& getComponentDefinitions() const;

   const String& getClassName() const;
   void          setClassName(const String& name);

   const Vector& getOffset() const;
   void          setOffset(const Vector& offset);

 // registration
   void addComponentDefinition(ComponentDefinition* pdefinition);
   void addChild(EntityDefinition* pdefinition);
   void addLink(EntityLinkDefinition* plink);

private:

 // data
   Children    mChildren;
   Components  mCompDefs;
   Links       mLinks;
   String      mClassName;
   Vector      mOffset;
};

#endif // ENTITY_DEFINITION_H
