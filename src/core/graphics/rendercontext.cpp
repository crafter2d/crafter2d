
#include "rendercontext.h"

namespace Graphics
{

RenderContext::RenderContext():
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

} // namespace Graphics
