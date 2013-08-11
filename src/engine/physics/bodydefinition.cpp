
#include "bodydefinition.h"

#ifndef JENGINE_INLINE
#  include "bodydefinition.inl"
#endif

BodyDefinition::BodyDefinition():
   mShapeType(eInvalidType),
   mInfoA(0.0f),
   mInfoB(0.0f),
   mStatic(false),
   mFixedRotation(false)
{
}

