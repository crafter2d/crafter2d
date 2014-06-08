
#ifndef PARTICLE_SYSTEM_RENDER_H
#define PARTICLE_SYSTEM_RENDER_H

#include <cstdint>
#include <vector>

#include "core/math/matrix4.h"
#include "core/math/vertex.h"

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
      typedef std::vector<const ParticleSystem*> ParticleSystems;

      struct ConstantBuffer
      {
         Matrix4 projection;
         Vertex  world;
      };

    // data
      ParticleSystems   mParticleSystems;
      ConstantBuffer    mConstants;

      Effect*           mpEffect;
	   uint32_t          mVertexBufferSize;
      VertexBuffer*     mpVertexBuffer;
      UniformBuffer*    mpUniformBuffer;
   };
}

#endif // PARTICLE_SYSTEM_RENDER_H
