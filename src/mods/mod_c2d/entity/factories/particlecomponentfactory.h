
#ifndef PARTICLE_COMPONENT_FACTORY_H
#define PARTICLE_COMPONENT_FACTORY_H

#include "componentfactory.h"

namespace Graphics
{
   class Device;
   class ParticleSystem;
}

class ParticleComponentDefinitionProto;

class ParticleComponentFactory : public ComponentFactory
{
public:
   ParticleComponentFactory(Graphics::Device& device);

 // overrides
   virtual Component* instantiate(const ComponentDefinitionProto& definition) const override;

private:

 // operations
   Graphics::ParticleSystem* instantiateParticleSystem(const ParticleComponentDefinitionProto& proto) const;

 // data
   Graphics::Device& mDevice;
};

#endif // PARTICLE_COMPONENT_FACTORY_H
