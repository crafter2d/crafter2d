
#ifndef ANIMATION_COMPONENT_MESSAGE_H
#define ANIMATION_COMPONENT_MESSAGE_H

#include "core/core_base.h"

#include "componentmessage.h"

class CORE_API AnimationComponentMessage : public ComponentMessage
{
public:
   AnimationComponentMessage();
   virtual ~AnimationComponentMessage();

   int  getAnimation() const
   {
      return mAnimation;
   }

   void setAnimation(int index)
   {
      mAnimation = index;
   }

private:

 // data
   int mAnimation;
};

#endif // ANIMATION_COMPONENT_MESSAGE_H
