
#ifndef ENTITY_DEFINITION_PROTO_H
#define ENTITY_DEFINITION_PROTO_H

#include <vector>

#include "core/string/string.h"

class ComponentFactories;
class Entity;
class DataStream;
class ChildDefinitionProto;
class LinkDefinitionProto;
class ComponentDefinitionProto;

class EntityDefinitionProto
{
public:
   typedef std::vector<ChildDefinitionProto*> Children;
   typedef std::vector<LinkDefinitionProto*> Links;
   typedef std::vector<EntityDefinitionProto*> Entities;
   typedef std::vector<ComponentDefinitionProto*> Components;

   Entity* instantiate(ComponentFactories& factories);

   void read(DataStream& stream);
   void write(DataStream& stream) const;

private:
 // search
   EntityDefinitionProto& resolve(const String& name);

 // data
   Children    mChildren;
   Links       mLinks;
   Entities    mEntities;
   Components  mComponents;
   String      mName;
   String      mClassName;
};

#endif // ENTITY_DEFINITION_PROTO_H
