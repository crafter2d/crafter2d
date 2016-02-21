
#include "texturewriter.h"

#include <memory>
#include <squish.h>

#include "core/graphics/image.h"
#include "core/streams/arraystream.h"

bool TextureWriter::write(DataStream& stream, const String& filename)
{
   c2d::Image image;
   if ( image.load(filename) )
   {
      stream.writeInt(image.getWidth());
      stream.writeInt(image.getHeight());
      stream.writeInt(image.getFormat());

      if ( image.getFormat() != 4 )
      {
         int size = image.getWidth() * image.getHeight() * image.getFormat();
         stream.writeBlob(image.getBytes(), size);
      }
      else
      {
         // https://technet.microsoft.com/en-us/subscriptions/downloads/ff471324(v=vs.85).aspx
         // libSquish is based on DX9 texture format specification
         // Based on the table for Texture2D the DXT5 maps to the new BC3 format

         int flags = squish::kDxt5 | squish::kColourClusterFit;
         int size = squish::GetStorageRequirements(image.getWidth(), image.getHeight(), flags);
         std::unique_ptr<unsigned char[]> data(new unsigned char[size]);
         squish::CompressImage(image.getBytes(), image.getWidth(), image.getHeight(), data.get(), flags);

         ArrayStream imagestream((char*)data.get(), size);
         stream.write(imagestream);
      }

      return true;
   }

   return false;
}
