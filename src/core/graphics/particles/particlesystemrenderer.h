
#ifndef PARTICLE_SYSTEM_RENDER_H
#define PARTICLE_SYSTEM_RENDER_H

#include <vector>

#include "particlerenderer.h"

namespace Graphics
{
   class Device;
   class Effect;
   class ParticleSystem;
   class RenderContext;
   class UniformBuffer;
   class VertexBuffer;
   class Viewport;

   class ParticleSystemRenderer
   {
   public:
      ParticleSystemRenderer();

    // operations
      bool create(Device& device);
      void destroy();

      void viewportChanged(RenderContext& context, const Viewport& viewport);
      void setOffset(RenderContext& context, const Vector& offset);

    // drawing
      void beginDraw(RenderContext& context);
      void drawParticles(const ParticleSystem& particles);
      void endDraw(RenderContext& context);

   private:
     
    // data
      c2d::IParticleRenderer::ParticleSystems  mParticleSystems;
      c2d::IParticleRenderer*                  mpRenderer;
   };
}

#endif // PARTICLE_SYSTEM_RENDER_H
