
#include "physicscomponentfactory.h"

#include "core/smartptr/autoptr.h"

#include "engine/physics/simulator.h"

#include "physicscomponent.h"
#include "physicscomponentdefinition.h"
#include "physicscomponentloader.h"

PhysicsComponentFactory::PhysicsComponentFactory(Simulator& simulator):
   ComponentFactory(ComponentInterface::ePhysisComponent),
   mSimulator(simulator)
{
}

// overrides
   
ComponentLoader* PhysicsComponentFactory::createLoader() const
{
   return new PhysicsComponentLoader();
}

Component* PhysicsComponentFactory::instantiate(const ComponentDefinition& definition) const
{
   const PhysicsComponentDefinition& physicsdef = static_cast<const PhysicsComponentDefinition&>(definition);
   const BodyDefinition& bodydefinition = physicsdef.getBodyDefinition();
         
   AutoPtr<PhysicsComponent> result = new PhysicsComponent();
   result->setBody(mSimulator.createBody(bodydefinition));
   return result.release();
}
