
#include "particlecomponentdefinitionproto.h"

#include "core/streams/datastream.h"

ParticleComponentDefinitionProto::ParticleComponentDefinitionProto():
   ComponentDefinitionProto(ComponentInterface::eParticleComponent),
   emitCount(0),
   emitRate(0.0f),
   gravity(0.0f),
   initSize(0.0f),
   initSizeRange(),
   initLifeTime(0.0f),
   initLifeTimeRange(),
   initVelocity(),
   initVelocityRange()
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
