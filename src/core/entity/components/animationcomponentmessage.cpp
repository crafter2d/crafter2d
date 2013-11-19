
#include "animationcomponentmessage.h"

AnimationComponentMessage::AnimationComponentMessage():
   ComponentMessage(ComponentInterface::eAnimationMsg),
   mAnimation(-1)
{
}

// - Get/set

int AnimationComponentMessage::getAnimation() const
{
   return mAnimation;
}

void AnimationComponentMessage::setAnimation(int index)
{
   mAnimation = index;
}
