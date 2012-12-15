
#include "rendercontext.h"

using namespace Graphics;

RenderContext::RenderContext():
   mViewport(),
   mClearColor()
{
}

void RenderContext::setViewport(const Viewport& viewport)
{
   mViewport = viewport;
}
