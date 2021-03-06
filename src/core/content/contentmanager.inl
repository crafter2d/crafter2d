
#include "core/defines.h"

namespace c2d
{
   INLINE Graphics::Device& ContentManager::getDevice()
   {
      ASSERT_PTR(mpDevice);
      return *mpDevice;
   }

   INLINE void ContentManager::setDevice(Graphics::Device& device)
   {
      mpDevice = &device;
   }

   INLINE Simulator& ContentManager::getSimulator()
   {
      ASSERT_PTR(mpSimulator);
      return *mpSimulator;
   }

   INLINE void ContentManager::setSimulator(Simulator& simulator)
   {
      mpSimulator = &simulator;
   }

   INLINE SoundManager& ContentManager::getSoundManager()
   {
      ASSERT_PTR(mpSoundManager);
      return *mpSoundManager;
   }

   INLINE void ContentManager::setSoundManager(SoundManager& manager)
   {
      mpSoundManager = &manager;
   }

   INLINE const String& ContentManager::getBaseDir() const
   {
      return mBaseDir;
   }

   INLINE void ContentManager::setBaseDir(const String& basedir)
   {
      mBaseDir = basedir;
   }
}
