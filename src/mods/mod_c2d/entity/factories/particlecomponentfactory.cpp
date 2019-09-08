
#include "particlecomponentfactory.h"

#include "core/entity/components/particlecomponent.h"
#include "core/graphics/particles/particlesystem.h"

#include "../proto/particlecomponentdefinitionproto.h"

using namespace Graphics;

ParticleComponentFactory::ParticleComponentFactory(Graphics::Device& device):
   ComponentFactory(ComponentInterface::eParticleComponent),
   mDevice(device)
{
}

// - Overrides

Component* ParticleComponentFactory::instantiate(const ComponentDefinitionProto& definition) const
{
   const ParticleComponentDefinitionProto& proto = static_cast<const ParticleComponentDefinitionProto&>(definition);

   ParticleSystem* psystem = instantiateParticleSystem(proto);

   ParticleComponent* pcomponent = new ParticleComponent();
   pcomponent->setParticleSystem(psystem);

   return pcomponent;
}

Graphics::ParticleSystem* ParticleComponentFactory::instantiateParticleSystem(const ParticleComponentDefinitionProto& proto) const
{
   ParticleSystem* presult = new ParticleSystem();

   presult->setEmitCount(proto.emitCount);
   presult->setEmitRate(proto.emitRate);
   presult->setGravity(proto.gravity);
   presult->setMaxActive(proto.max);

   presult->setInitAreaRange(proto.initAreaRange);
   presult->setInitSize(proto.initSize);
   presult->setInitSizeRange(proto.initSizeRange);
   presult->setInitLifeTime(proto.initLifeTime);
   presult->setInitLifeTimeRange(proto.initLifeTimeRange);
   presult->setInitVelocity(proto.initVelocity);
   presult->setInitVelocityRange(proto.initVelocityRange);

   presult->create(mDevice);

   return presult;
}
