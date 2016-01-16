
#include "particlesystemrenderer.h"

#include "core/defines.h"
#include "core/graphics/blendstate.h"
#include "core/graphics/device.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/uniformbuffer.h"
#include "core/graphics/vertexbuffer.h"

#include "particle.h"
#include "particlesystem.h"
#include "particlerendererdefault.h"

namespace Graphics
{
   ParticleSystemRenderer::ParticleSystemRenderer():
      mParticleSystems(),
      mpRenderer(NULL)
   {
   }

   // - Operations

   bool ParticleSystemRenderer::create(Device& device)
   {
      mpRenderer = new c2d::ParticleRendererDefault();

      return true;
   }

   void ParticleSystemRenderer::destroy()
   {
      delete mpRenderer;
      mpRenderer = nullptr;
   }

   void ParticleSystemRenderer::viewportChanged(RenderContext& context, const Viewport& viewport)
   {
      mpRenderer->viewportChanged(context, viewport);
   }

   void ParticleSystemRenderer::setOffset(RenderContext& context, const Vector& offset)
   {
      mpRenderer->setOffset(context, offset);
   }

   // - Drawing

   void ParticleSystemRenderer::beginDraw(RenderContext& context)
   {
      C2D_UNUSED(context);
      
      mParticleSystems.clear();
   }

   void ParticleSystemRenderer::drawParticles(const ParticleSystem& particlesystem)
   {
      mParticleSystems.push_back(&particlesystem);
   }
   
   void ParticleSystemRenderer::endDraw(RenderContext& context)
   {
      mpRenderer->render(context, mParticleSystems);
   }
}
