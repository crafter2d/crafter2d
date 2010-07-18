
#include "box2dcontactlistener.h"

#include "box2dbody.h"

Box2DContactListener::Box2DContactListener():
   b2ContactListener()
{
}

void Box2DContactListener::BeginContact(b2Contact* contact)
{
   Box2DBody* pbodyA = (Box2DBody*)contact->GetFixtureA()->GetBody()->GetUserData();
   Box2DBody* pbodyB = (Box2DBody*)contact->GetFixtureB()->GetBody()->GetUserData();
}
