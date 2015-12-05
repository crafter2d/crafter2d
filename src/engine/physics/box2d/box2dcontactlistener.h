
#ifndef BOX2D_CONTACT_LISTENER
#define BOX2D_CONTACT_LISTENER

#include <Box2D/Box2D.h>

class Box2DSimulator;

/**
\author J. Broekhuizen
\brief Box2D contact listener.
\details This listener is called by the Box2D library in case a contact between 2 objects have been detected.
*/
class Box2DContactListener : public b2ContactListener
{
public:
   Box2DContactListener(Box2DSimulator& simulator);

 // overloads
   virtual void BeginContact(b2Contact* pcontact);
   virtual void EndContact(b2Contact* pcontact);

private:
 // operations
   void collision(b2Contact* pcontact, bool begin);
   void collisionObjectWorld(b2Contact* pcontact, b2Fixture& object, b2Fixture& bound, bool begin);
   void collisionObjectObject(b2Contact* pcontact, b2Fixture& sensor, b2Fixture& object, bool begin);

   Box2DSimulator& mSimulator;
};

#endif
