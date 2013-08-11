
#ifndef BODY_DEFINITION_H
#define BODY_DEFINITION_H

class Body;

class BodyDefinition
{
public:

   enum ShapeType
   {
      eCircle,
      eBox,
      eInvalidType
   };

   BodyDefinition();

 // get/set
   ShapeType getShapeType() const;
   void      setShapeType(ShapeType type);

   float getRadius() const;
   void  setRadius(float radius);

   float getWidth() const;
   void  setWidth(float width);

   float getHeight() const;
   void  setHeight(float height);

   bool isStatic() const;
   void setStatic(bool isstatic);

   bool isFixedRotation() const;
   void setFixedRotation(bool fixed);
   
private:

 // data
   ShapeType   mShapeType;
   float       mInfoA;     // used for radius and dimension
   float       mInfoB;
   bool        mStatic;
   bool        mFixedRotation;
};

#ifdef JENGINE_INLINE
#  include "bodydefinition.h"
#endif

#endif // BODY_DEFINITION_H
