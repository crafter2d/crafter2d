
#ifndef PARTICLE_RENDERER_H
#define PARTICLE_RENDERER_H

#include <vector>

namespace Graphics
{
   class Device;
   class ParticleSystem;
   class RenderContext;
   class Viewport;
}

class Vector;

namespace c2d
{
   class IParticleRenderer
   {
   public:
      using ParticleSystems = std::vector<const Graphics::ParticleSystem*>;

      virtual bool create(Graphics::Device& device) = 0;
      virtual void render(Graphics::RenderContext& context, const ParticleSystems& systems) = 0;
      virtual void viewportChanged(Graphics::RenderContext& context, const Graphics::Viewport& viewport) = 0;
      virtual void setOffset(Graphics::RenderContext& context, const Vector& offset) = 0;
   };
}

#endif // PARTICLE_RENDERER_H

