
#ifndef ANIMATION_COMPONENT_MESSAGE_H
#define ANIMATION_COMPONENT_MESSAGE_H

#include "core/core_base.h"

#include "componentmessage.h"

class CORE_API AnimationComponentMessage : public ComponentMessage
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
