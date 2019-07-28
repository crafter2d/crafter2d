
#include "rendercontext.h"

namespace Graphics
{

RenderContext::RenderContext():
   mpTileAtlas(nullptr),
   mSpriteRenderer(),
   mParticleRenderer(),
   mTextRenderer(),
   mClearColor(),
   mDpi(96.0f)
{
}

RenderContext::~RenderContext()
{
   delete mpTileAtlas;
}

// - Get/set

void RenderContext::viewportChanged(const Viewport& viewport)
{
   mSpriteRenderer.viewportChanged(*this, viewport);
   mParticleRenderer.viewportChanged(*this, viewport);
   mTextRenderer.viewportChanged(*this, viewport);
}

// - Initialization

bool RenderContext::initialize(Device& device)
{
   return mParticleRenderer.create(device)
       && mSpriteRenderer.create(device)
       && mTextRenderer.initialize(device);
}

void RenderContext::destroy()
{
   mParticleRenderer.destroy();
}

// - Rendering

void RenderContext::beginDraw()
{
   mSpriteRenderer.beginDraw(*this);
   mParticleRenderer.beginDraw(*this);
}

void RenderContext::endDraw()
{
   mSpriteRenderer.endDraw(*this);
   mParticleRenderer.endDraw(*this);
   mTextRenderer.render(*this);
}

void RenderContext::setSpriteOffset(const Vector& offset)
{
   mSpriteRenderer.setOffset(*this, offset);
   mParticleRenderer.setOffset(*this, offset);
}

void RenderContext::drawSprite(const c2d::Sprite& sprite)
{
   mSpriteRenderer.draw(sprite);
}

void RenderContext::drawParticles(const ParticleSystem& particles)
{
   mParticleRenderer.drawParticles(particles);
}

void RenderContext::drawText(const TextLayout& textlayout)
{
   mTextRenderer.draw(textlayout);
}

} // namespace Graphics
