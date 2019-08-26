
#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include "core/physics/bodylistener.h"

#include "component.h"

class Body;

class CORE_API PhysicsComponent : public Component, IBodyListener
{
public:
   PhysicsComponent();
   virtual ~PhysicsComponent();

 // get/set
   bool              hasBody() const;
   const Body&       getBody() const;
         Body&       getBody();
   void              setBody(Body& body);

 // component interface
   virtual void registerComponent(Components& components) override;
   virtual void handleMessage(ComponentMessage& message) override;

 // notifications
   virtual void onPositionChanged(Body& body);

private:
   Body*    mpBody;
};

#ifdef JENGINE_INLINE
#  include "physicscomponent.inl"
#endif

#endif // ACTOR_PHYSICS_COMPONENT_H
