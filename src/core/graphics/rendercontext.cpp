
#include "rendercontext.h"

namespace Graphics
{

RenderContext::RenderContext():
   mTileAtlas(),
   mSpriteRenderer(),
   mParticleRenderer(),
   mTextRenderer(),
   mClearColor(),
   mDpi(96.0f)
{
}

RenderContext::~RenderContext()
{
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

void RenderContext::drawSprite(const c2d::Sprite& sprite)
{
   mSpriteRenderer.draw(sprite);
}

void RenderContext::drawParticles(const ParticleSystem& particles)
{
   mParticleRenderer.drawParticles(particles);
}

} // namespace Graphics
