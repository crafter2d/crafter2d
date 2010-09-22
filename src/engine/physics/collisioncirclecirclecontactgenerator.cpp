#include "collisioncirclecirclecontactgenerator.h"

#include "collisiondata.h"
#include "collisioncircle.h"
#include "collisioncontactbody.h"

CollisionCircleCircleContactGenerator::CollisionCircleCircleContactGenerator()
{
}

CollisionCircleCircleContactGenerator::~CollisionCircleCircleContactGenerator()
{
}

bool CollisionCircleCircleContactGenerator::collide(CollisionData& data, const CollisionShape& one, const CollisionShape& two)
{
   ASSERT(one.getType() == CollisionShape::eCircle);
   ASSERT(two.getType() == CollisionShape::eCircle);

   const CollisionCircle& circleone = static_cast<const CollisionCircle&>(one);
   const CollisionCircle& circletwo = static_cast<const CollisionCircle&>(two);

   const Vector& posOne = circleone.getPosition();
   const Vector& posTwo = circletwo.getPosition();

   Vector diff = posOne - posTwo;
   float distance = diff.length();
   if ( distance <= 0.0f || distance > circleone.getRadius() + circletwo.getRadius() )
   {
      return false;
   }

   CollisionContact* pcontact = new CollisionContact();
   pcontact->setLeft(new CollisionContactBody(*circleone.getBody()));
   pcontact->setRight(new CollisionContactBody(*circletwo.getBody()));

   pcontact->mPoint       = posOne + (diff * 0.5);
   pcontact->mNormal      = diff * (1.0f / distance);
   pcontact->mPenetration = circleone.getRadius() + circletwo.getRadius() - distance;

   data.addContact(pcontact);

   return true;
}
