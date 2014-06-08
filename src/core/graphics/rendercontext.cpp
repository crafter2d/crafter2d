
#include "rendercontext.h"

namespace Graphics
{

RenderContext::RenderContext():
   mSpriteRenderer(),
   mParticleRenderer(),
   mTextRenderer(),
   mViewport(),
   mClearColor()
{
}

// - Get/set

void RenderContext::setViewport(const Viewport& viewport)
{
   mViewport = viewport;

   mSpriteRenderer.viewportChanged(*this, viewport);
   mParticleRenderer.viewportChanged(*this, viewport);
   mTextRenderer.viewportChanged(*this, viewport);

   onViewportChanged(viewport);
}

// - Initialization

void RenderContext::initialize(Device& device)
{
   mParticleRenderer.create(device);
   mSpriteRenderer.create(device);
   mTextRenderer.initialize(device);
}

void RenderContext::destroy()
{
   mParticleRenderer.destroy();
}

// - Drawing

void RenderContext::drawText(const Vector& position, Font& font, float fontsizeem, const String& text)
{
   mTextRenderer.draw(*this, position, font, fontsizeem, text);
}

// - Sprites

void RenderContext::beginDraw()
{
   mSpriteRenderer.beginDraw(*this);
   mParticleRenderer.beginDraw(*this);
}

void RenderContext::endDraw()
{
   mSpriteRenderer.endDraw(*this);
   mParticleRenderer.endDraw(*this);
}

void RenderContext::setSpriteOffset(const Vector& offset)
{
   mSpriteRenderer.setOffset(*this, offset);
   mParticleRenderer.setOffset(*this, offset);
}

void RenderContext::drawSprite(const Sprite& sprite)
{
   mSpriteRenderer.draw(sprite);
}

void RenderContext::drawParticles(const ParticleSystem& particles)
{
   mParticleRenderer.drawParticles(particles);
}

} // namespace Graphics
