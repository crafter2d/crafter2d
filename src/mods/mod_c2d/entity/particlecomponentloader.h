
#ifndef PARTICLE_COMPONENT_LOADER_H
#define PARTICLE_COMPONENT_LOADER_H

#include <string>

#include "componentloader.h"

class ParticleComponentDefinitionProto;
class Range;
class TiXmlElement;
class Vector;
class VectorRange;

class ParticleComponentLoader : public ComponentLoader
{
public:
   ParticleComponentLoader();

 // overrides
   virtual const String& getXmlTag() const override;
   virtual ComponentDefinitionProto* load(const TiXmlElement& element) override;

private:

 // loading
   bool loadInit(const TiXmlElement& initElement, ParticleComponentDefinitionProto& proto);
   bool loadUpdate(const TiXmlElement& initElement, ParticleComponentDefinitionProto& proto);

   float loadValue(const TiXmlElement& xmlElement, const char* pname);
   Range loadValueRange(const TiXmlElement& xmlElement, const char* pname);

   Vector      loadVector(const TiXmlElement& xmlElement, const char* pname);
   VectorRange loadVectorRange(const TiXmlElement& xmlElement, const char* pname);

   Vector parseVector(const std::string& value);
};

#endif // PARTICLE_COMPONENT_LOADER_H
