
#include "particlecomponentdefinitionproto.h"

#include "core/streams/datastream.h"

ParticleComponentDefinitionProto::ParticleComponentDefinitionProto():
   ComponentDefinitionProto(ComponentInterface::eParticleComponent)
{
}

ParticleComponentDefinitionProto::~ParticleComponentDefinitionProto()
{
}

// - Overrides

void ParticleComponentDefinitionProto::virRead(DataStream& stream)
{
   stream >> emitCount >> emitRate >> gravity;

   // init settings
   stream >> initSize >> initSizeRange
      >> initLifeTime >> initLifeTimeRange
      >> initVelocity >> initVelocityRange;

   // update settings
}
   
void ParticleComponentDefinitionProto::virWrite(DataStream& stream) const
{
   stream << emitCount << emitRate << gravity;

   // init settings
   stream << initSize << initSizeRange 
      << initLifeTime << initLifeTimeRange
      << initVelocity << initVelocityRange;

   // update settings
}
