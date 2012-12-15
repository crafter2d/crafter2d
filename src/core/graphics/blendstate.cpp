
#include "blendstate.h"

namespace Graphics
{

BlendState::BlendState(bool enabled):
   mEnabled(enabled)
{
}

BlendState::~BlendState()
{
}

// get/set

bool BlendState::isEnabled() const
{
   return mEnabled;
}

void BlendState::setEnabled(bool enabled)
{
   mEnabled = enabled;
}

};
