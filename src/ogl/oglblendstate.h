
#ifndef OGL_BLEND_STATE_H
#define OGL_BLEND_STATE_H

#include "core/graphics/blendstate.h"

#include <GL/GLee.h>

namespace Graphics
{

class OGLBlendState : public BlendState
{
public:
   OGLBlendState(GLenum sfactor, GLenum dfactor, bool enabled);

 // get/set
   GLenum getSourceFactor() const;
   GLenum getDestFactor() const;

private:
   GLenum mSourceFactor;
   GLenum mDestFactor;
};

};

#endif // OGL_BLEND_STATE_H
