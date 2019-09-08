
#ifndef MESH_COMPONENT_LOADER_H
#define MESH_COMPONENT_LOADER_H

#include "xml/xmlparser/xmlparts.h"

class ComponentDefinitionProto;
class EntityDefinitionProto;

namespace c2d
{
   using namespace xml;

   namespace entity_definitions
   {
      ELEMENT_DEF(tile)
         ATTRIBUTE(std::string, name)
      ELEMENT_IMP(tile)
         ATTRIBUTE_INIT(name, Attribute::eMandatory, Attribute::eString, "")
      ELEMENT_END

      ELEMENT_DEF(anim)
         ATTRIBUTE(std::string, name)
         CHILD(tile)
      ELEMENT_IMP(anim)
         ATTRIBUTE_INIT(name, Attribute::eMandatory, Attribute::eString, "")
         CHILD_INIT(tile, Child::eOneOrAny)
      ELEMENT_END

      ELEMENT_DEF(animations)
         ATTRIBUTE(float, speed)
         CHILD(anim)
      ELEMENT_IMP(animations)
         ATTRIBUTE_INIT(speed, Attribute::eOptional, Attribute::eFloat, 100.0f)
         CHILD_INIT(anim, Child::eOneOrAny)
      ELEMENT_END

      ELEMENT_DEF(texture)
         ATTRIBUTE(std::string, name)
      ELEMENT_IMP(texture)
         ATTRIBUTE_INIT(name, Attribute::eMandatory, Attribute::eString, "")
      ELEMENT_END

      ELEMENT_DEF(sprite)
         ATTRIBUTE(int, width)
         ATTRIBUTE(int, height)
         CHILD(animations)
         CHILD(texture)
      ELEMENT_IMP(sprite)
         ATTRIBUTE_INIT(width, Attribute::eMandatory, Attribute::eInt, 0)
         ATTRIBUTE_INIT(height, Attribute::eMandatory, Attribute::eInt, 0)
         CHILD_INIT(animations, Child::eOne)
         CHILD_INIT(texture, Child::eZeroOrOne)
      ELEMENT_END
   }

   class MeshComponentLoader
   {
   public:
      static void load(EntityDefinitionProto& entity, const std::vector<entity_definitions::sprite>& sprites);
      
   private:
      MeshComponentLoader();
      ComponentDefinitionProto* load(const entity_definitions::sprite& sprite);
   };
}

#endif // MESH_COMPONENT_LOADER_H
