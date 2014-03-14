
#include "soundreader.h"

#include "core/graphics/device.h"
#include "core/graphics/texture.h"
#include "core/streams/bufferedstream.h"
#include "core/sound/soundmanager.h"
#include "core/sound/sound.h"
#include "core/defines.h"

IContent* SoundReader::read(DataStream& stream)
{
   BufferedStream datastream;
   stream.read(datastream);

   if ( hasSoundManager() )
   {
      return getSoundManager().createSound(datastream);
   }

   return NULL;
}
