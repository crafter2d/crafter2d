
#include "rendercontext.h"

namespace Graphics
{

RenderContext::RenderContext():
   mSpriteRenderer(),
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
   mTextRenderer.viewportChanged(*this, viewport);

   onViewportChanged(viewport);
}

// - Initialization

void RenderContext::initialize(Device& device)
{
   mSpriteRenderer.create(device);
   mTextRenderer.initialize(device);
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
}

void RenderContext::endDraw()
{
   mSpriteRenderer.endDraw(*this);
}

void RenderContext::setSpriteOffset(const Vector& offset)
{
   mSpriteRenderer.setOffset(*this, offset);
}

void RenderContext::drawSprite(const Sprite& sprite)
{
   mSpriteRenderer.draw(sprite);
}

} // namespace Graphics
