
#include "contentreader.h"

#include "core/defines.h"

namespace c2d
{
   ContentReader::ContentReader() :
      mpContentManager(nullptr),
      mpDevice(nullptr),
      mpSimulator(nullptr),
      mpSoundManager(nullptr)
   {
   }
   
   ContentReader::~ContentReader()
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
      return mpDevice != nullptr;
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
      return mpSimulator != nullptr;
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
      return mpSoundManager != nullptr;
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
