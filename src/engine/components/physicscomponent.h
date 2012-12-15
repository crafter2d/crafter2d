
#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "core/math/vector.h"

#include "component.h"

class Body;

class PhysicsComponent : public Component
{
public:
   PhysicsComponent();

 // get/set
   bool              hasBody() const;
   const Body&       getBody() const;
         Body&       getBody();
   void              setBody(Body& body);

 // component interface
   virtual void handleMessage(const ComponentMessage& message);
   virtual void update(float delta);

private:
   Body*    mpBody;
   Vector   mPosition;
   float    mAngle;
};

#ifdef JENGINE_INLINE
#  include "physicscomponent.inl"
#endif

#endif // ACTOR_PHYSICS_COMPONENT_H
