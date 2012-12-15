
#include "oglblendstate.h"

namespace Graphics
{

OGLBlendState::OGLBlendState(GLenum sfactor, GLenum dfactor, bool enabled):
   BlendState(enabled),
   mSourceFactor(sfactor),
   mDestFactor(dfactor)
{
}


// - Get/set

GLenum OGLBlendState::getSourceFactor() const
{
   return mSourceFactor;
}

GLenum OGLBlendState::getDestFactor() const
{
   return mDestFactor;
}

};
