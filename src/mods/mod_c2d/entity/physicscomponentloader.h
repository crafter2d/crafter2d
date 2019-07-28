
#ifndef PHYSICS_COMPONENT_LOADER_H
#define PHYSICS_COMPONENT_LOADER_H

#include "xml/xmlparser/xmlparts.h"

class ComponentDefinitionProto;
class EntityDefinitionProto;

namespace c2d
{
   using namespace xml;

   namespace entity_definitions
   {
      ELEMENT_DEF(shape)
         ATTRIBUTE(std::string, type)
         ATTRIBUTE(float, halfx)
         ATTRIBUTE(float, halfy)
         ATTRIBUTE(float, radius)
      ELEMENT_IMP(shape)
         ATTRIBUTE_INIT(type, Attribute::eMandatory, Attribute::eString, "")
         ATTRIBUTE_INIT(halfx, Attribute::eOptional, Attribute::eFloat, 0.0f)
         ATTRIBUTE_INIT(halfy, Attribute::eOptional, Attribute::eFloat, 0.0f)
         ATTRIBUTE_INIT(radius, Attribute::eOptional, Attribute::eFloat, 0.0f)
      ELEMENT_END

      ELEMENT_DEF(body)
         ATTRIBUTE(float, inertia)
         ATTRIBUTE(float, mass)
         ATTRIBUTE(bool, statik)
         ATTRIBUTE(bool, rotate)
         CHILD(shape)
      ELEMENT_IMP(body)
         ATTRIBUTE_INIT(inertia, Attribute::eOptional, Attribute::eFloat, 0.0f)
         ATTRIBUTE_INIT(mass, Attribute::eMandatory, Attribute::eFloat, 0.0f)
         ATTRIBUTE_INIT_NAMED(statik, static, Attribute::eOptional, Attribute::eBool, false)
         ATTRIBUTE_INIT(rotate, Attribute::eOptional, Attribute::eBool, true);
         CHILD_INIT(shape, Child::eOne, false)
      ELEMENT_END
   }

   class PhysicsComponentLoader
   {
   public:
      static void load(EntityDefinitionProto& entity, const std::vector<entity_definitions::body>& bodies);

   private:

      // overrides
      ComponentDefinitionProto* load(const entity_definitions::body& element);
   };
}
#endif // PHYSICS_COMPONENT_LOADER_H
