
#ifndef CONTENT_READER_H
#define CONTENT_READER_H

#include "core/core_base.h"

namespace Graphics
{
   class Device;
}

class IContent;
class DataStream;
class Simulator;

namespace c2d
{
   class ContentManager;
   class SoundManager;

   class CORE_API ContentReader
   {
   public:
      ContentReader();
      virtual ~ContentReader();

      // get/set
      ContentManager& getContentManager();
      void            setContentManager(ContentManager& manager);

      bool              hasGraphicsDevice() const;
      Graphics::Device& getGraphicsDevice();
      void              setGraphicsDevice(Graphics::Device* device);

      bool       hasPhysicsSimulator() const;
      Simulator& getPhysicsSimulator();
      void       setPhysicsSimulator(Simulator* psimulator);

      bool          hasSoundManager();
      SoundManager& getSoundManager();
      void          setSoundManager(SoundManager* psoundmanager);

      /// Reads and instantiates the content from disc
      virtual IContent* read(DataStream& stream) = 0;

   private:

      // data
      ContentManager*   mpContentManager;
      Graphics::Device* mpDevice;
      Simulator*        mpSimulator;
      SoundManager*     mpSoundManager;
   };
}

#endif // CONTENT_READER_H
