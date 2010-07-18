
#ifndef BOX2D_CONTACT_LISTENER
#define BOX2D_CONTACT_LISTENER

#include <Box2D.h>

class Box2DContactListener : public b2ContactListener
{
public:
   Box2DContactListener();

   void BeginContact(b2Contact* contact);
};

#endif
