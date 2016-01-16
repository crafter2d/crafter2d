#include "particlerenderergeometry.h"

#include "core/graphics/blendstate.h"
#include "core/graphics/blendstatedesc.h"
#include "core/graphics/device.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/uniformbuffer.h"
#include "core/graphics/vertexbuffer.h"
#include "core/graphics/viewport.h"

#include "particle.h"
#include "particlesystem.h"

using namespace Graphics;

struct ParticleVertex {
   Vertex pos;
   Color  col;
   float  radius;
};

c2d::ParticleRendererGeometry::ParticleRendererGeometry():
   mConstants(),
   mpEffect(nullptr),
   mVertexBufferSize(0),
   mpVertexBuffer(nullptr),
   mpUniformBuffer(nullptr)
{
}



bool c2d::ParticleRendererGeometry::create(Graphics::Device& device)
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
}

void c2d::ParticleRendererGeometry::destroy()
{
   if ( mpUniformBuffer != NULL )
   {
      delete mpUniformBuffer;
      mpUniformBuffer = NULL;
   }
}

void c2d::ParticleRendererGeometry::render(Graphics::RenderContext& context, const ParticleSystems& systems)
{
   mpEffect->enable(context);
   mpEffect->setConstantBuffer(context, 0, *mpUniformBuffer);

   context.setVertexBuffer(*mpVertexBuffer);

   for ( auto psystem : systems )
   {
      int particleCount = psystem->getActiveParticleCount();

      if ( particleCount > 0 )
      {
         const Particles& particles = psystem->getActiveParticles();
         ParticleVertex* verts = reinterpret_cast<ParticleVertex*>(mpVertexBuffer->lock(context));

         for ( int pidx = 0; pidx < particleCount; ++pidx )
         {
            auto particle = particles[pidx];

            ParticleVertex& v = verts[pidx];
            v.pos = particle.pos;
            v.col = particle.color;
            v.radius = particle.size * 0.5f;
         }

         mpVertexBuffer->unlock(context);

         context.setTexture(0, psystem->getTexture());
         context.drawPoints(0, particleCount);
      }
   }
}

void c2d::ParticleRendererGeometry::viewportChanged(RenderContext& context, const Viewport & viewport)
{
   mConstants.projection.setOrtho(viewport.getWidth(), viewport.getHeight(), -1, 1);

   mpUniformBuffer->set(context, &mConstants, sizeof(mConstants));
}

void c2d::ParticleRendererGeometry::setOffset(Graphics::RenderContext & context, const Vector & offset)
{
   if ( mConstants.world.x != offset.x || mConstants.world.y != offset.y )
   {
      mConstants.world.set(offset.x, offset.y);
      mpUniformBuffer->set(context, &mConstants, sizeof(mConstants));
   }
}
