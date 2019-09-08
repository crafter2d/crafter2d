
#ifndef PARTICLE_COMPONENT_DEFINITION_PROTO_H
#define PARTICLE_COMPONENT_DEFINITION_PROTO_H

#include "core/math/range.h"
#include "core/math/vector.h"
#include "core/math/vectorrange.h"

#include "componentdefinitionproto.h"

class ParticleComponentDefinitionProto : public ComponentDefinitionProto
{
public:
   ParticleComponentDefinitionProto();
   virtual ~ParticleComponentDefinitionProto();

 // data
   int   emitCount;
   float emitRate;
   float gravity;
   int   max;

   Vector      initArea;
   VectorRange initAreaRange;
   float       initSize;
   Range       initSizeRange;
   float       initLifeTime;
   Range       initLifeTimeRange;
   Vector      initVelocity;
   VectorRange initVelocityRange;
   

protected:
 // storage
   virtual void virRead(DataStream& stream) override;
   virtual void virWrite(DataStream& stream) const override;

 // data
   
};

#endif // PARTICLE_COMPONENT_DEFINITION_PROTO_H
