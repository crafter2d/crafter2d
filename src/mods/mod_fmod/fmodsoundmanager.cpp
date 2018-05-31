
#include "fmodsoundmanager.h"

#include <string.h>
#include <fmod.hpp>
#include <fmod_errors.h>

#include "core/streams/datastream.h"
#include "core/string/string.h"
#include "core/defines.h"

#include "fmodsound.h"

#define SUCCEEEDED(r)(r == FMOD_OK)
#define FAILED(r)(r != FMOD_OK)

namespace c2d
{

   FModSoundManager::FModSoundManager() :
      SoundManager(),
      mpSystem(nullptr)
   {
   }

   bool FModSoundManager::initialize()
   {
      FMOD_RESULT result = FMOD::System_Create(&mpSystem);
      if ( result != FMOD_OK )
      {
         return false;
      }

      unsigned int version;
      mpSystem->getVersion(&version);
      if ( version < FMOD_VERSION )
      {
         // wrong DLL version
         return false;
      }

      /*FMOD_SPEAKERMODE speakermode = FMOD_SPEAKERMODE_STEREO;
      result = mpSystem->getDriverCaps(0, nullptr, nullptr, &speakermode);
      if ( FAILED(result) )
      {
         return false;
      }

      result = mpSystem->setSpeakerMode(speakermode);
      if ( FAILED(result) )
      {
         return false;
      }*/

      result = mpSystem->init(32, FMOD_INIT_NORMAL, 0);
      if ( FAILED(result) )
      {
         return false;
      }

      return true;
   }

   void FModSoundManager::destroy()
   {
      FMOD_RESULT result = mpSystem->close();
      if ( FAILED(result) )
      {
         // what to do? :P
      }

      mpSystem->release();
   }

   void FModSoundManager::update()
   {
      mpSystem->update();
   }

   // - Factory functions

   Sound* FModSoundManager::createSound(const String& filename) const
   {
      FMOD::Sound* psound = nullptr;
      FMOD_RESULT result = mpSystem->createSound(filename.toUtf8().c_str(), FMOD_DEFAULT, 0, &psound);
      if ( SUCCEEEDED(result) )
      {
         return new FModSound(psound);
      }
      return nullptr;
   }

   Sound* FModSoundManager::createSound(const DataStream& stream) const
   {
      FMOD_CREATESOUNDEXINFO info;
      memset(&info, 0, sizeof(FMOD_CREATESOUNDEXINFO));
      info.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
      info.length = stream.getDataSize();

      FMOD::Sound* psound = nullptr;
      FMOD_RESULT result = mpSystem->createSound(stream.getData(), FMOD_DEFAULT | FMOD_OPENMEMORY, &info, &psound);
      if ( SUCCEEEDED(result) )
      {
         return new FModSound(psound);
      }
      return nullptr;
   }

   // - Media player functions

   bool FModSoundManager::play(const Sound& sound)
   {
      const FModSound& fmodsound = static_cast<const FModSound&>(sound);
      FMOD_RESULT result = mpSystem->playSound(fmodsound.mpSound, 0, false, &fmodsound.mpChannel);
      return SUCCEEEDED(result);
   }

} // namespace c2d
