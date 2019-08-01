
#include "texturereader.h"

#include "core/graphics/device.h"
#include "core/graphics/texture.h"
#include "core/streams/bufferedstream.h"

IContent* TextureReader::read(DataStream& stream)
{
   if ( !hasGraphicsDevice() )
   {
      return nullptr;
   }

   int width, height, format;
   stream >> width >> height >> format;

   BufferedStream datastream;
   stream.read(datastream);

   Graphics::TextureDescription desc;
   desc.width = width;
   desc.height = height;
   desc.format = (Graphics::TextureFormat)format;
   desc.pinitData = datastream.getData();

   return getGraphicsDevice().createTexture(desc);
}
