
#ifndef PHYSICS_COMPONENT_FACTORY_H
#define PHYSICS_COMPONENT_FACTORY_H

#include "componentfactory.h"

class Simulator;

class PhysicsComponentFactory : public ComponentFactory
{
public:
   PhysicsComponentFactory(Simulator& simulator);

 // overrides
   virtual ComponentLoader* createLoader() const override;
   virtual Component* instantiate(const ComponentDefinition& definition) const override;


private:

 // data
   Simulator& mSimulator;
};

#endif // PHYSICS_COMPONENT_FACTORY_H
