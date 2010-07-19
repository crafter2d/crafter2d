
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
   const b2Manifold* pmanifold = pcontact->GetManifold();
   if ( pmanifold->pointCount == 0 )
      return;

   b2Body* pbodyA = pcontact->GetFixtureA()->GetBody();
   b2Body* pbodyB = pcontact->GetFixtureB()->GetBody();

   if ( pbodyA->GetUserData() == NULL && pbodyB->GetUserData() != NULL )
   {
      collisionObjectWorld(pcontact, *pcontact->GetFixtureB(), *pcontact->GetFixtureA());
   }
   else if ( pbodyA->GetUserData() != NULL && pbodyB->GetUserData() == NULL )
   {
      collisionObjectWorld(pcontact, *pcontact->GetFixtureA(), *pcontact->GetFixtureB());
   }
}

void Box2DContactListener::EndContact(b2Contact* pcontact)
{
   // no manifold information, we will have to do with contact & fixtures
   b2Body* pbodyA = pcontact->GetFixtureA()->GetBody();
   b2Body* pbodyB = pcontact->GetFixtureB()->GetBody();

   if ( pbodyA->GetUserData() == NULL && pbodyB->GetUserData() != NULL )
   {
      finishCollisionObjectWorld(pcontact, *pcontact->GetFixtureB(), *pcontact->GetFixtureA());
   }
   else if ( pbodyA->GetUserData() != NULL && pbodyB->GetUserData() == NULL )
   {
      finishCollisionObjectWorld(pcontact, *pcontact->GetFixtureA(), *pcontact->GetFixtureB());
   }
}

void Box2DContactListener::collisionObjectWorld(b2Contact* pcontact, b2Fixture& object, b2Fixture& bound)
{
   // collision between a object(A) && bound(B)

   b2WorldManifold worldManifold;
   pcontact->GetWorldManifold(&worldManifold);

   b2Vec2 vel = object.GetBody()->GetLinearVelocityFromWorldPoint(worldManifold.points[0]);
   if ( vel.y > 0 )
   {
      if ( mSimulator.hasListener() )
      {
         Box2DBody* pbody = (Box2DBody*)object.GetBody()->GetUserData();
         Bound* pbound    = (Bound*)bound.GetUserData();

         mSimulator.getListener().collideObjectWorld(pbody->getObject(), *pbound, true);
      }
   }
}

void Box2DContactListener::finishCollisionObjectWorld(b2Contact* pcontact, b2Fixture& object, b2Fixture& bound)
{
   if ( mSimulator.hasListener() )
   {
      Box2DBody* pbody = (Box2DBody*)object.GetBody()->GetUserData();
      Bound* pbound    = (Bound*)bound.GetUserData();

      mSimulator.getListener().collideObjectWorld(pbody->getObject(), *pbound, false);
   }
}
