
#ifndef PARTICLE_RENDERER_GEOMETRY_H
#define PARTICLE_RENDERER_GEOMETRY_H

#include <cstdint>

#include "core/math/matrix4.h"
#include "core/math/vertex.h"

#include "particlerenderer.h"

namespace Graphics
{
   class Effect;
   class VertexBuffer;
   class UniformBuffer;
}

namespace c2d
{
   class ParticleRendererGeometry : public IParticleRenderer
   {
   public:
      ParticleRendererGeometry();
      virtual ~ParticleRendererGeometry();

      virtual bool create(Graphics::Device& device) override;
      virtual void render(Graphics::RenderContext& context, const ParticleSystems& systems) override;
      virtual void viewportChanged(Graphics::RenderContext& context, const Graphics::Viewport& viewport) override;
      virtual void setOffset(Graphics::RenderContext& context, const Vector& offset) override;

   private:
      struct ConstantBuffer
      {
         Matrix4 projection;
         Vertex  world;
      };

      void destroy();

    // data
      ConstantBuffer             mConstants;
      Graphics::Effect*          mpEffect;
      uint32_t                   mVertexBufferSize;
      Graphics::VertexBuffer*    mpVertexBuffer;
      Graphics::UniformBuffer*   mpUniformBuffer;
   };
}

#endif // PARTICLE_RENDERER_GEOMETRY_H

