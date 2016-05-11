
#include "particlecomponentloader.h"

#include <memory>
#include <tinyxml.h>

#include "core/string/string.h"
#include "core/defines.h"

#include "proto/particlecomponentdefinitionproto.h"

static const String sXmlTag("particlesystem");

ParticleComponentLoader::ParticleComponentLoader():
   ComponentLoader()
{
}

// - Overrides
   
const String& ParticleComponentLoader::getXmlTag() const
{
   return sXmlTag;
}

ComponentDefinitionProto* ParticleComponentLoader::load(const TiXmlElement& element)
{
   std::unique_ptr<ParticleComponentDefinitionProto> proto(new ParticleComponentDefinitionProto());

   if ( element.QueryFloatAttribute("emitrate", &proto->emitRate) != TIXML_SUCCESS )
   {
      return NULL;
   }

   proto->gravity = loadValue(element, "gravity");

   const TiXmlElement* pXmlInitElement = element.FirstChildElement("init");
   if ( pXmlInitElement != NULL )
   {
      loadInit(*pXmlInitElement, *proto);
   }

   const TiXmlElement* pXmlUpdateElement = element.FirstChildElement("update");
   if ( pXmlUpdateElement != NULL )
   {
      loadUpdate(*pXmlUpdateElement, *proto);
   }

   return proto.release();
}

bool ParticleComponentLoader::loadInit(const TiXmlElement& initElement, ParticleComponentDefinitionProto& proto)
{
   const TiXmlElement* pXmlElement = initElement.FirstChildElement("size");
   if ( pXmlElement != NULL )
   {
      proto.initSize = loadValue(*pXmlElement, "value");
      proto.initSizeRange = loadValueRange(*pXmlElement, "range");
   }

   pXmlElement = initElement.FirstChildElement("lifetime");
   if ( pXmlElement != NULL )
   {
      proto.initLifeTime = loadValue(*pXmlElement, "value");
      proto.initLifeTimeRange = loadValueRange(*pXmlElement, "range");
   }

   pXmlElement = initElement.FirstChildElement("velocity");
   if ( pXmlElement != NULL )
   {
      proto.initVelocity = loadVector(*pXmlElement, "value");
      proto.initVelocityRange = loadVectorRange(*pXmlElement, "range");
   }

   return true;
}

bool ParticleComponentLoader::loadUpdate(const TiXmlElement& updateElement, ParticleComponentDefinitionProto& proto)
{
   C2D_UNUSED(proto);
   
   const TiXmlElement* pXmlElement = updateElement.FirstChildElement("size");
   if ( pXmlElement != NULL )
   {
   }

   pXmlElement = updateElement.FirstChildElement("velocity");
   if ( pXmlElement != NULL )
   {
   }

   return true;
}

float ParticleComponentLoader::loadValue(const TiXmlElement& xmlElement, const char* pname)
{
   float result = 0.0f;
   if ( xmlElement.QueryFloatAttribute(pname, &result) == TIXML_WRONG_TYPE )
   {
      throw new std::runtime_error("Invalid type!");
   }
   return result;
}

Range ParticleComponentLoader::loadValueRange(const TiXmlElement& xmlElement, const char* pname)
{
   Range result;
   std::string value;
   if ( xmlElement.QueryStringAttribute(pname, &value) == TIXML_SUCCESS )
   {
      std::size_t index = value.find(':');
      if ( index != value.npos )
      {
         std::string leftstr = value.substr(0, index);
         std::string rightstr = value.substr(index+1, value.length() - index - 1);

         float left = (float) std::atof(leftstr.c_str());
         float right = (float) std::atof(rightstr.c_str());

         result.set(left, right - left);
      }
   }
   return result;
}

Vector ParticleComponentLoader::loadVector(const TiXmlElement& xmlElement, const char* pname)
{
   Vector result;
   std::string value;
   if ( xmlElement.QueryStringAttribute(pname, &value) == TIXML_SUCCESS )
   {
      result = parseVector(value);
   }

   return result;
}

VectorRange ParticleComponentLoader::loadVectorRange(const TiXmlElement& xmlElement, const char* pname)
{
   VectorRange result;
   std::string value;
   if ( xmlElement.QueryStringAttribute(pname, &value) == TIXML_SUCCESS )
   {
      std::size_t index = value.find(':');
      if ( index != value.npos )
      {
         std::string leftrange = value.substr(0, index);
         std::string rightrange = value.substr(index+1, value.length() - index - 1);

         Vector left = parseVector(leftrange);
         Vector right = parseVector(rightrange);

         result.set(left, right);
      }
   }

   return result;
}

Vector ParticleComponentLoader::parseVector(const std::string& value)
{
   Vector result;
   std::size_t index = value.find(',');
   if ( index != std::string::npos )
   {
      std::string x = value.substr(0, index);
      result.x = (float) std::atof(x.c_str());

      std::string y = value.substr(index+1, value.length() - index - 1);
      result.y = (float) std::atof(y.c_str());
   }
   return result;
}
