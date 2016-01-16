
#ifndef PARTICLE_RENDERER_DEFAULT_H
#define PARTICLE_RENDERER_DEFAULT_H

#include <cstdint>

#include "particlerenderer.h"

namespace Graphics
{
   class Effect;
   class IndexBuffer;
   class VertexBuffer;
}

namespace c2d
{
   class ParticleRendererDefault : public IParticleRenderer
   {
   public:
      ParticleRendererDefault();
      virtual ~ParticleRendererDefault();

      virtual bool create(Graphics::Device& device) override;
      virtual void render(Graphics::RenderContext& context, const ParticleSystems& systems) override;
      virtual void viewportChanged(Graphics::RenderContext& context, const Graphics::Viewport& viewport) override;
      virtual void setOffset(Graphics::RenderContext& context, const Vector& offset) override;

   private:

    // data
      Graphics::Effect*        mpEffect;
      uint32_t                 mVertexBufferSize;
      Graphics::VertexBuffer*  mpVertexBuffer;
      Graphics::IndexBuffer*   mpIndexBuffer;
   };
}

#endif // PARTICLE_RENDERER_DEFAULT_H

