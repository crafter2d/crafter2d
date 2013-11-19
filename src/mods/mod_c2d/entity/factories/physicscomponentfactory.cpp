
#include "physicscomponentfactory.h"

#include "core/entity/components/physicscomponent.h"
#include "core/physics/bodydefinition.h"
#include "core/smartptr/autoptr.h"
#include "core/physics/simulator.h"

#include "../proto/physicscomponentdefinitionproto.h"

PhysicsComponentFactory::PhysicsComponentFactory(Simulator& simulator):
   ComponentFactory(ComponentInterface::ePhysisComponent),
   mSimulator(simulator)
{
}

// overrides

Component* PhysicsComponentFactory::instantiate(const ComponentDefinitionProto& definition) const
{
   const PhysicsComponentDefinitionProto& physicsdef = static_cast<const PhysicsComponentDefinitionProto&>(definition);
   

   BodyDefinition bodydef;
   bodydef.setShapeType(physicsdef.mShape);

         
   AutoPtr<PhysicsComponent> result = new PhysicsComponent();
   result->setBody(mSimulator.createBody(bodydefinition));
   return result.release();
}
