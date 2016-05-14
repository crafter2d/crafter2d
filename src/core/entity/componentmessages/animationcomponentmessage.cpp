
#include "animationcomponentmessage.h"

AnimationComponentMessage::AnimationComponentMessage():
   ComponentMessage(ComponentInterface::eAnimationMsg),
   mAnimation(-1)
{
}

AnimationComponentMessage::~AnimationComponentMessage()
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
