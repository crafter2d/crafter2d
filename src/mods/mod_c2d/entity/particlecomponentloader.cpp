
#include "particlecomponentloader.h"

#include <memory>

#include "core/string/string.h"
#include "core/defines.h"

#include "proto/entitydefinitionproto.h"
#include "proto/particlecomponentdefinitionproto.h"

namespace c2d
{
   void ParticleComponentLoader::load(EntityDefinitionProto& entity, const std::vector<entity_definitions::particlesystem>& systems)
   {
      ParticleComponentLoader loader;
      for ( auto& system : systems )
      {
         auto pcomponent = loader.load(system);
         entity.mComponents.push_back(pcomponent);
      }
   }

   ComponentDefinitionProto* ParticleComponentLoader::load(const entity_definitions::particlesystem& element)
   {
      auto proto = std::make_unique<ParticleComponentDefinitionProto>();
      proto->emitRate = element.emitrate;
      proto->gravity = element.gravity;

      if ( !element.inits.empty() )
      {
         loadParticleBehavior(*proto, element.inits[0]);
      }

      if ( !element.updates.empty() )
      {
         loadParticleBehavior(*proto, element.updates[0]);
      }

      return proto.release();
   }

   bool ParticleComponentLoader::loadParticleBehavior(ParticleComponentDefinitionProto& proto, const entity_definitions::particlebehavior& behavior)
   {
      if ( !behavior.sizes.empty() )
      {
         auto& size = behavior.sizes[0];
         proto.initSize = parseFloat(size.value);
         proto.initSizeRange = parseRange(size.range);
      }

      if ( !behavior.lifetimes.empty() )
      {
         auto& lifetime = behavior.lifetimes[0];
         proto.initLifeTime = parseFloat(lifetime.value);
         proto.initLifeTimeRange = parseRange(lifetime.range);
      }

      if ( !behavior.velocitys.empty() )
      {
         auto& velocity = behavior.velocitys[0];
         proto.initVelocity = parseVector(velocity.value);
         proto.initVelocityRange = parseVectorRange(velocity.range);
      }

      return true;
   }

   float ParticleComponentLoader::parseFloat(const std::string& value)
   {
      return (float)std::atof(value.c_str());
   }

   Range ParticleComponentLoader::parseRange(const std::string& value)
   {
      std::size_t index = value.find(':');
      if ( index == value.npos )
      {
         throw new std::runtime_error("Invalid range detected, expected 'from:to'");
      }

      std::string leftstr = value.substr(0, index);
      std::string rightstr = value.substr(index + 1, value.length() - index - 1);

      float left = (float)std::atof(leftstr.c_str());
      float right = (float)std::atof(rightstr.c_str());

      return Range(left, right - left);
   }

   Vector ParticleComponentLoader::parseVector(const std::string& value)
   {
      std::size_t index = value.find(',');
      if ( index == std::string::npos )
      {
         throw new std::runtime_error("Invalid vector detected, expected 'x,y' value");
      }

      std::string x = value.substr(0, index);
      std::string y = value.substr(index + 1, value.length() - index - 1);

      Vector result;
      result.x = (float)std::atof(x.c_str());
      result.y = (float)std::atof(y.c_str());
      return result;
   }

   VectorRange ParticleComponentLoader::parseVectorRange(const std::string& value)
   {
      std::size_t index = value.find(':');
      if ( index == value.npos )
      {
         throw new std::runtime_error("Invalid vector range, expected 'from:to'");
      }

      std::string leftrange = value.substr(0, index);
      std::string rightrange = value.substr(index + 1, value.length() - index - 1);

      Vector left = parseVector(leftrange);
      Vector right = parseVector(rightrange);

      return VectorRange(left, right);
   }
}
