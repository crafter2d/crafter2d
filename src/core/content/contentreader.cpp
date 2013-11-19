
#include "contentreader.h"

#include "core/defines.h"

IContentReader::IContentReader():
   mpDevice(NULL),
   mpSimulator(NULL)
{
}

// - Get/set

bool IContentReader::hasGraphicsDevice() const
{
   return mpDevice != NULL;
}

Graphics::Device& IContentReader::getGraphicsDevice()
{
   ASSERT_PTR(mpDevice);
   return *mpDevice;
}

void IContentReader::setGraphicsDevice(Graphics::Device& device)
{
   mpDevice = &device;
}

bool IContentReader::hasPhysicsSimulator() const
{
   return mpSimulator != NULL;
}

Simulator& IContentReader::getPhysicsSimulator()
{
   ASSERT_PTR(mpSimulator);
   return *mpSimulator;
}

void IContentReader::setPhysicsSimulator(Simulator& simulator)
{
   mpSimulator = &simulator;
}
