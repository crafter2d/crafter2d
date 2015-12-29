
#include "core/defines.h"

namespace Graphics
{

inline const Color& RenderContext::getClearColor() const
{
   return mClearColor;
}
   
inline void RenderContext::setClearColor(const Color& color)
{
   mClearColor = color;
}

inline TextRenderer& RenderContext::getTextRenderer()
{
   return mTextRenderer;
}

};