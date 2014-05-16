
#include "particlesystemrenderer.h"

#include "core/defines.h"
#include "core/graphics/blendstate.h"
#include "core/graphics/device.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/uniformbuffer.h"
#include "core/graphics/vertexbuffer.h"

#include "particle.h"
#include "particlesystem.h"

struct ParticleVertex {
	Vertex pos;
   Color  col;
   float  radius;
};

namespace Graphics
{
   ParticleSystemRenderer::ParticleSystemRenderer():
      mParticles(),
      mConstants(),
      mpEffect(NULL),
      mVertexBufferSize(0),
      mpVertexBuffer(NULL),
      mpUniformBuffer(NULL)
   {
   }

   // - Operations

   bool ParticleSystemRenderer::create(Device& device)
   {
      // load effect
      mpEffect = device.createEffect(UTEXT("shaders/particle"));
      if ( mpEffect == NULL )
      {
         return false;
      }

      // create the blend state
      BlendStateDesc desc(BlendStateDesc::BS_SRC_ALPHA, BlendStateDesc::BS_ONE, true);
      BlendState* pblendstate = device.createBlendState(desc);
      if ( pblendstate == NULL )
      {
         return false;
      }

      mpEffect->setBlendState(pblendstate);

      mVertexBufferSize = 2048;
      int usage = VertexBuffer::eDynamic | VertexBuffer::eWriteOnly;
      mpVertexBuffer = mpEffect->createVertexBuffer(device, mVertexBufferSize, usage);
      if ( mpVertexBuffer == NULL )
	   {
         return false;
      }

      UNIFORM_BUFFER_DESC descs[] = {
         { UTEXT("proj"), sizeof(float)* 16 },
         { UTEXT("world"), sizeof(float)* 16 },
         { UTEXT("object"), sizeof(float)* 16 },
      };

      mpUniformBuffer = mpEffect->createUniformBuffer(UTEXT("mpv"));
      if ( mpUniformBuffer == NULL || !mpUniformBuffer->create(device, descs, 3) )
      {
         return false;
      }

      mConstants.projection.setIdentity();
      mConstants.world.setIdentity();
      mConstants.object.setIdentity();

      return true;
   }

   void ParticleSystemRenderer::destroy()
   {
      if ( mpEffect != NULL )
      {
         delete mpEffect;
         mpEffect = NULL;
      }

      if ( mpVertexBuffer != NULL )
      {
         delete mpVertexBuffer;
         mpVertexBuffer = NULL;
      }

      if ( mpUniformBuffer != NULL )
      {
         delete mpUniformBuffer;
         mpUniformBuffer = NULL;
      }
   }

   void ParticleSystemRenderer::viewportChanged(RenderContext& context, const Viewport& viewport)
   {
      mConstants.projection.setOrtho(viewport.getWidth(), viewport.getHeight(), -1, 1);

      mpUniformBuffer->set(context, &mConstants);
   }

   // - Drawing

   void ParticleSystemRenderer::beginDraw(RenderContext& context)
   {
      mParticles.clear();
   }

   void ParticleSystemRenderer::drawParticles(const ParticleSystem& particles)
   {
      mParticles.push_back(&particles);
   }
   
   void ParticleSystemRenderer::endDraw(RenderContext& context)
   {
      mpEffect->enable(context);
      mpEffect->setConstantBuffer(context, 0, *mpUniformBuffer);

      context.setVertexBuffer(*mpVertexBuffer);
     
      uint32_t num = 0;
      ParticleVertex* verts = reinterpret_cast<ParticleVertex*>(mpVertexBuffer->lock(context));
      for ( std::size_t index = 0; index < mParticles.size(); ++index )
      {
         const ParticleSystem& system = *mParticles[index];
         const Particle* part = system.getActiveParticles();

         context.setTexture(0, system.getTexture());
         
         while ( part != NULL )
         {
            while ( part != NULL && num < mVertexBufferSize )
            {
               ParticleVertex& v = verts[num++];
               v.pos = part->pos;
               v.col = part->color;
               v.radius = part->size * 0.5f;

               part = part->next;
            }

            if ( part != NULL )
            {
               // buffer full -> render contents and continue
               mpVertexBuffer->unlock(context);

               context.drawPoints(0, num);

               verts = reinterpret_cast<ParticleVertex*>(mpVertexBuffer->lock(context));
               num = 0;
            }
         }

         mpVertexBuffer->unlock(context);

         if ( num > 0 )
         {
            context.drawPoints(0, num);
         }
      }
   }
}
