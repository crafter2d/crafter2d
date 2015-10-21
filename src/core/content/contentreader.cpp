
#include "contentreader.h"

#include "core/defines.h"

namespace c2d
{
   ContentReader::ContentReader() :
      mpContentManager(NULL),
      mpDevice(NULL),
      mpSimulator(NULL),
      mpSoundManager(NULL)
   {
   }

   // - Get/set

   ContentManager& ContentReader::getContentManager()
   {
      ASSERT_PTR(mpContentManager);
      return *mpContentManager;
   }

   void ContentReader::setContentManager(ContentManager& manager)
   {
      mpContentManager = &manager;
   }

   bool ContentReader::hasGraphicsDevice() const
   {
      return mpDevice != NULL;
   }

   Graphics::Device& ContentReader::getGraphicsDevice()
   {
      ASSERT_PTR(mpDevice);
      return *mpDevice;
   }

   void ContentReader::setGraphicsDevice(Graphics::Device* pdevice)
   {
      mpDevice = pdevice;
   }

   bool ContentReader::hasPhysicsSimulator() const
   {
      return mpSimulator != NULL;
   }

   Simulator& ContentReader::getPhysicsSimulator()
   {
      ASSERT_PTR(mpSimulator);
      return *mpSimulator;
   }

   void ContentReader::setPhysicsSimulator(Simulator* psimulator)
   {
      mpSimulator = psimulator;
   }

   bool ContentReader::hasSoundManager()
   {
      return mpSoundManager != NULL;
   }

   SoundManager& ContentReader::getSoundManager()
   {
      ASSERT_PTR(mpSoundManager);
      return *mpSoundManager;
   }

   void ContentReader::setSoundManager(SoundManager* psoundmanager)
   {
      mpSoundManager = psoundmanager;
   }
}
