
#include "box2dfactory.h"

#include "box2dsimulator.h"

static const String sBox2DFactoryName = UTEXT("box2d");

Box2DFactory::Box2DFactory()
{
}

Box2DFactory::~Box2DFactory()
{
}

const String& Box2DFactory::getName() const
{
   return sBox2DFactoryName;
}

Simulator* Box2DFactory::createSimulator()
{
   return new Box2DSimulator();
}

bool Box2DFactory::isCreatedBy(const Simulator& simulator) const
{
   const Box2DSimulator* pbox2dsim = dynamic_cast<const Box2DSimulator*>(&simulator);
   return pbox2dsim != NULL;
}

