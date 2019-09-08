
#ifndef PARTICLE_COMPONENT_LOADER_H
#define PARTICLE_COMPONENT_LOADER_H

#include <string>

#include "xml/xmlparser/xmlparts.h"

class ComponentDefinitionProto;
class EntityDefinitionProto;
class ParticleComponentDefinitionProto;

class Range;
class Vector;
class VectorRange;

namespace c2d
{
   using namespace xml;

   namespace entity_definitions
   {
      ELEMENT_DEF(initvalue)
         ATTRIBUTE(std::string, value)
         ATTRIBUTE(std::string, range)
      ELEMENT_IMP(initvalue)
         ATTRIBUTE_INIT(value, Attribute::eMandatory, Attribute::eString, "")
         ATTRIBUTE_INIT(range, Attribute::eMandatory, Attribute::eString, "")
      ELEMENT_END

      ELEMENT_DEF(particlebehavior)
         CHILD_NAMED(initvalue, area)
         CHILD_NAMED(initvalue, lifetime)
         CHILD_NAMED(initvalue, size)
         CHILD_NAMED(initvalue, velocity)
      ELEMENT_IMP(particlebehavior)
         CHILD_INIT_NAMED(initvalue, area, Child::eZeroOrOne);
         CHILD_INIT_NAMED(initvalue, lifetime, Child::eZeroOrOne)
         CHILD_INIT_NAMED(initvalue, size, Child::eZeroOrOne)
         CHILD_INIT_NAMED(initvalue, velocity, Child::eZeroOrOne)
      ELEMENT_END

      ELEMENT_DEF(emit)
         ATTRIBUTE(int, count)
         ATTRIBUTE(int, rate)
         ATTRIBUTE(int, max)
      ELEMENT_IMP(emit)
         ATTRIBUTE_INIT(count, Attribute::eOptional, Attribute::eInt, 10)
         ATTRIBUTE_INIT(rate, Attribute::eMandatory, Attribute::eInt, 0)
         ATTRIBUTE_INIT(max, Attribute::eOptional, Attribute::eInt, 100)
      ELEMENT_END

      ELEMENT_DEF(particlesystem)
         ATTRIBUTE(float, gravity)
         CHILD(emit)
         CHILD_NAMED(particlebehavior, init)
         CHILD_NAMED(particlebehavior, update)
      ELEMENT_IMP(particlesystem)
         ATTRIBUTE_INIT(gravity, Attribute::eMandatory, Attribute::eFloat, 0)
         CHILD_INIT(emit, Child::eOne, false)
         CHILD_INIT_NAMED(particlebehavior, init, Child::eZeroOrOne)
         CHILD_INIT_NAMED(particlebehavior, update, Child::eZeroOrOne)
      ELEMENT_END
   }

   class ParticleComponentLoader
   {
   public:

      static void load(EntityDefinitionProto& entity, const std::vector<entity_definitions::particlesystem>& systems);

   private:

    // loading
      ComponentDefinitionProto* load(const entity_definitions::particlesystem& element);

    // loading
      bool loadParticleBehavior(ParticleComponentDefinitionProto& proto, const entity_definitions::particlebehavior& behavior);
      bool loadParticleUpdates(ParticleComponentDefinitionProto& proto, const entity_definitions::particlebehavior& behavior);

    // parsing
      float parseFloat(const std::string& value);
      Range parseRange(const std::string& value);
      Vector parseVector(const std::string& value);
      VectorRange parseVectorRange(const std::string& value);
   };
}

#endif // PARTICLE_COMPONENT_LOADER_H
