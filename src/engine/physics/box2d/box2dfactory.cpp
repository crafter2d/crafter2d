
#include "box2dfactory.h"

#include "box2dsimulator.h"

static const std::string sBox2DFactoryName = "box2d";

Box2DFactory::Box2DFactory()
{
}

Box2DFactory::~Box2DFactory()
{
}

const std::string& Box2DFactory::getName() const
{
   return sBox2DFactoryName;
}

Simulator* Box2DFactory::createSimulator()
{
   return new Box2DSimulator();
}
