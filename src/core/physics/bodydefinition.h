
#ifndef BODY_DEFINITION_H
#define BODY_DEFINITION_H

#include "core/core_base.h"

class Body;

class CORE_API BodyDefinition
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

   float getMass() const {
      return mMass;
   }

   void setMass(float mass) {
      mMass = mass;
   }

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

 // operations
   void createBox(float width, float height);
   void createCircle(float radius);
   
private:

 // data
   ShapeType   mShapeType;
   float       mMass;
   float       mInfoA;     // used for radius and dimension
   float       mInfoB;
   bool        mStatic;
   bool        mFixedRotation;
};

#ifdef JENGINE_INLINE
#  include "bodydefinition.h"
#endif

#endif // BODY_DEFINITION_H
