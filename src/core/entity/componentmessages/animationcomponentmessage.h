
#ifndef ANIMATION_COMPONENT_MESSAGE_H
#define ANIMATION_COMPONENT_MESSAGE_H

#include "componentmessage.h"

class AnimationComponentMessage : public ComponentMessage
{
public:
   AnimationComponentMessage();

   int  getAnimation() const;
   void setAnimation(int index);

private:

 // data
   int mAnimation;
};

#endif // ANIMATION_COMPONENT_MESSAGE_H
