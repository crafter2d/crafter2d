
#include "animationcomponentmessage.h"

AnimationComponentMessage::AnimationComponentMessage():
   ComponentMessage(ComponentInterface::eAnimationMsg),
   mAnimation(-1)
{
}

AnimationComponentMessage::~AnimationComponentMessage()
{
}
