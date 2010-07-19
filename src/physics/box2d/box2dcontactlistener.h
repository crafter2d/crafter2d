
#ifndef BOX2D_CONTACT_LISTENER
#define BOX2D_CONTACT_LISTENER

#include <Box2D.h>

class Box2DSimulator;

class Box2DContactListener : public b2ContactListener
{
public:
   Box2DContactListener(Box2DSimulator& simulator);

   virtual void BeginContact(b2Contact* pcontact);
   virtual void EndContact(b2Contact* pcontact);

private:
   void collisionObjectWorld(b2Contact* pcontact, b2Fixture& object, b2Fixture& bound);
   void finishCollisionObjectWorld(b2Contact* pcontact, b2Fixture& object, b2Fixture& bound);

   Box2DSimulator& mSimulator;
};

#endif
