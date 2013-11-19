
#ifndef BODY_LISTENER_H
#define BODY_LISTENER_H

#include "core/core_base.h"

class Body;

class CORE_API IBodyListener
{
public:
   virtual void onPositionChanged(Body& body) = 0;
};

#endif // BODY_LISTENER_H
