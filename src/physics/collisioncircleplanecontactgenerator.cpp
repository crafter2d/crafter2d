#include "collisioncircleplanecontactgenerator.h"

#include "collisiondata.h"
#include "collisioncircle.h"
#include "collisionplane.h"

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

   CollisionContact* pcontact = new CollisionContact();
   pcontact->mpLeft       = circle.getBody();
   pcontact->mpRight      = plane.getBody();
   pcontact->mPoint       = position - plane.getNormal() * (distance + circle.getRadius());
   pcontact->mNormal      = plane.getNormal();
   pcontact->mPenetration = -distance;

   data.addContact(pcontact);

   return true;
}
