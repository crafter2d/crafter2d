
#ifndef ENTITY_LOADER_H
#define ENTITY_LOADER_H

#include <vector>

#include "core/string/string.h"

class ChildDefinitionProto;
class EntityDefinitionProto;
class LinkDefinitionProto;
class TiXmlElement;

namespace c2d
{
   namespace entity_definitions
   {
      class entity;
      class link;
      class child;
   }

   class EntityLoader
   {
   public:
      EntityLoader();

      // loading
      EntityDefinitionProto* load(const String& filename);

   private:

      // loading
      EntityDefinitionProto* createDefinition(const entity_definitions::entity& ent);

      void loadEntityDefinitions(EntityDefinitionProto& entity, const std::vector<entity_definitions::entity>& entities);
      void loadChildDefinitions(EntityDefinitionProto& entity, const std::vector<entity_definitions::child>& childelement);
      void loadLinkDefinitions(EntityDefinitionProto& entity, const std::vector<entity_definitions::link>& xmllink);
   };
}

#endif // ENTITY_LOADER_H
