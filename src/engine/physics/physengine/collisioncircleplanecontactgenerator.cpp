#include "collisioncircleplanecontactgenerator.h"

#include "collisiondata.h"
#include "collisioncircle.h"
#include "collisionplane.h"
#include "collisioncontactbody.h"

CollisionCirclePlaneContactGenerator::CollisionCirclePlaneContactGenerator(void)
{
}

CollisionCirclePlaneContactGenerator::~CollisionCirclePlaneContactGenerator(void)
{
}

bool CollisionCirclePlaneContactGenerator::collide(CollisionData& data, const CollisionShape& one, const CollisionShape& two)
{
   if ( one.getType() == CollisionShape::eCircle )
   {
      return docollide(data, static_cast<const CollisionCircle&>(one), static_cast<const CollisionPlane&>(two));
   }
   else
   {
      return docollide(data, static_cast<const CollisionCircle&>(two), static_cast<const CollisionPlane&>(one));
   }
}

bool CollisionCirclePlaneContactGenerator::docollide(CollisionData& data, const CollisionCircle& circle, const CollisionPlane& plane)
{
   const Vector& position = circle.getPosition();

   float distance = plane.getNormal().dot(position)
                  - circle.getRadius()
                  - plane.getOffset();
   if ( distance >= 0.0f )
   {
      return false;
   }

   Vector collisionpoint = position - plane.getNormal() * (distance + circle.getRadius());
   if ( !plane.hitTest(collisionpoint) )
   {
      return false;
   }

   CollisionContact* pcontact = new CollisionContact();
   pcontact->setLeft(new CollisionContactBody(*circle.getBody()));
   if ( plane.getBody() != NULL )
      pcontact->setRight(new CollisionContactBody(*plane.getBody()));

   pcontact->mPoint       = collisionpoint;
   pcontact->mNormal      = plane.getNormal();
   pcontact->mPenetration = -distance;

   data.addContact(pcontact);

   return true;
}
