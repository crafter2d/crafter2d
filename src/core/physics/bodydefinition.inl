
#include "core/defines.h"

INLINE BodyDefinition::ShapeType BodyDefinition::getShapeType() const
{
   return mShapeType;
}

INLINE void BodyDefinition::setShapeType(ShapeType type)
{
   mShapeType = type;
}

INLINE float BodyDefinition::getRadius() const
{
   return mInfoA;
}

INLINE void BodyDefinition::setRadius(float radius)
{
   mInfoA = radius;
}

INLINE float BodyDefinition::getWidth() const
{
   return mInfoA;
}
   
INLINE void BodyDefinition::setWidth(float width)
{
   mInfoA = width;
}

INLINE float BodyDefinition::getHeight() const
{
   return mInfoB;
}

INLINE void BodyDefinition::setHeight(float height)
{
   mInfoB = height;
}

INLINE bool BodyDefinition::isStatic() const
{
   return mStatic;
}

INLINE void BodyDefinition::setStatic(bool isstatic)
{
   mStatic = isstatic;
}

INLINE bool BodyDefinition::isFixedRotation() const
{
   return mFixedRotation;
}
   
INLINE void BodyDefinition::setFixedRotation(bool fixed)
{
   mFixedRotation = fixed;
}
