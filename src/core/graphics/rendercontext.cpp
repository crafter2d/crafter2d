
#include "rendercontext.h"

namespace Graphics
{

RenderContext::RenderContext():
   mSpriteRenderer(),
   mViewport(),
   mClearColor()
{
}

// - Get/set

void RenderContext::setViewport(const Viewport& viewport)
{
   mViewport = viewport;

   onViewportChanged(viewport);
}

// - Initialization

void RenderContext::initialize(Device& device)
{
   mSpriteRenderer.create(device);
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

void RenderContext::drawSprite(const Sprite& sprite)
{
   mSpriteRenderer.draw(sprite);
}

} // namespace Graphics
