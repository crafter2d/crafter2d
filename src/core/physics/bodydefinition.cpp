
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

// - Operations

void BodyDefinition::createBox(float width, float height)
{
   mShapeType = BodyDefinition::eBox;
   mInfoA = width;
   mInfoB = height;
}

void BodyDefinition::createCircle(float radius)
{
   mShapeType = BodyDefinition::eCircle;
   mInfoA = radius;
}
