#include "particlerendererdefault.h"

c2d::ParticleRendererDefault::ParticleRendererDefault() :
   mpEffect(nullptr),
   mVertexBufferSize(0),
   mpVertexBuffer(nullptr),
   mpIndexBuffer(nullptr)
{
}

c2d::ParticleRendererDefault::~ParticleRendererDefault()
{
}

bool c2d::ParticleRendererDefault::create(Graphics::Device & device)
{
   return false;
}

void c2d::ParticleRendererDefault::render(Graphics::RenderContext& context, const ParticleSystems& systems)
{
   for ( auto psystem : systems )
   {

   }
}

void c2d::ParticleRendererDefault::viewportChanged(Graphics::RenderContext & context, const Graphics::Viewport & viewport)
{
}

void c2d::ParticleRendererDefault::setOffset(Graphics::RenderContext & context, const Vector & offset)
{
}
