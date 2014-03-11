
#include "fmodsoundmanager.h"

#include <fmod.hpp>
#include <fmod_errors.h>

#include "core/string/string.h"
#include "core/defines.h"

#include "fmodsound.h"

#define SUCCEEEDED(r)(r == FMOD_OK)
#define FAILED(r)(r != FMOD_OK)

namespace c2d
{

   SoundManager::SoundManager():
      mpSystem(NULL)
   {
   }

   bool SoundManager::initialize()
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

      FMOD_SPEAKERMODE speakermode = FMOD_SPEAKERMODE_STEREO;
      result = mpSystem->getDriverCaps(0, NULL, NULL, &speakermode);
      if ( FAILED(result) )
      {
         return false;
      }

      result = mpSystem->setSpeakerMode(speakermode);
      if ( FAILED(result) )
      {
         return false;
      }

      result = mpSystem->init(32, FMOD_INIT_NORMAL, 0);
      if ( FAILED(result) )
      {
         return false;
      }

      return true;
   }

   void SoundManager::destroy()
   {
      FMOD_RESULT result = mpSystem->close();
      if ( FAILED(result) )
      {
         // what to do? :P
      }

      mpSystem->release();
   }

   void SoundManager::update()
   {
      mpSystem->update();
   }

   // - Factory functions

   Sound* SoundManager::createSound(const String& filename) const
   {
      FMOD::Sound* psound = NULL;
      FMOD_RESULT result = mpSystem->createSound(filename.toUtf8().c_str(), FMOD_HARDWARE, 0, &psound);
      if ( SUCCEEEDED(result) )
      {
         return new Sound(psound);
      }
      return NULL;
   }

   Sound* SoundManager::createTrack(const String& filename) const
   {
      FMOD::Sound* psound = NULL;
      FMOD_RESULT result = mpSystem->createSound(filename.toUtf8().c_str(), FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &psound);
      if ( SUCCEEEDED(result) )
      {
         return new Sound(psound);
      }
      return NULL;
   }

   // - Media player functions

   bool SoundManager::play(const Sound& sound)
   {
      FMOD_RESULT result = mpSystem->playSound(FMOD_CHANNEL_FREE, sound.mpSound, false, &sound.mpChannel);
      return SUCCEEEDED(result);
   }

} // namespace c2d
