
#include "box2dcontactlistener.h"

#include "physics/simulatorlistener.h"

#include "world/bound.h"

#include "box2dbody.h"
#include "box2dsimulator.h"

Box2DContactListener::Box2DContactListener(Box2DSimulator& simulator):
   b2ContactListener(),
   mSimulator(simulator)
{
}

void Box2DContactListener::BeginContact(b2Contact* pcontact)
{
   collision(pcontact, true);
}

void Box2DContactListener::EndContact(b2Contact* pcontact)
{
   collision(pcontact, false);
}

void Box2DContactListener::collision(b2Contact* pcontact, bool begin)
{
   b2Fixture* pa = pcontact->GetFixtureA();
   b2Fixture* pb = pcontact->GetFixtureB();

   if ( pa->IsSensor() && !pb->IsSensor() )
   {
      collisionObjectWorld(pcontact, *pa, *pb, begin);
   }
   else if ( !pa->IsSensor() && pb->IsSensor() )
   {
      collisionObjectWorld(pcontact, *pb, *pa, begin);
   }
}

void Box2DContactListener::collisionObjectWorld(b2Contact* pcontact, b2Fixture& sensor, b2Fixture& bound, bool begin)
{
   // collision between a object(A) && bound(B)

   Box2DBody* pbody = (Box2DBody*)sensor.GetUserData();
   int side = pbody->getSide(sensor);
   
   if ( side > 0 && mSimulator.hasListener() )
   {
      mSimulator.getListener().collideObjectWorld(pbody->getObject(), *(Bound*)bound.GetUserData(), side, begin);
   }
}
