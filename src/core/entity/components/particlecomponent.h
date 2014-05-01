
#ifndef PARTICLE_COMPONENT_H
#define PARTICLE_COMPONENT_H

#include "component.h"

namespace Graphics
{
   class ParticleSystem;
   class RenderContext;
}

class CORE_API ParticleComponent : public Component
{
public:
   ParticleComponent();

 // overrides
   virtual void registerComponent(Components& components) override;
   virtual void handleMessage(ComponentMessage& message) override;

private:

 // operations
   void update(float delta);
   void render(Graphics::RenderContext& context);

 // data
   Graphics::ParticleSystem* mpSystem;
};

#endif // PARTICLE_COMPONENT_H
