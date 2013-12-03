
#include "texturereader.h"

#include "core/graphics/device.h"
#include "core/graphics/texture.h"
#include "core/streams/bufferedstream.h"
#include "core/defines.h"

IContent* TextureReader::read(DataStream& stream)
{
   BufferedStream datastream;
   stream.read(datastream);

   if ( hasGraphicsDevice() )
   {
      return getGraphicsDevice().createTexture(datastream);
   }

   return NULL;
}
