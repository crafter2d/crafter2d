
#include "texturewriter.h"

#ifdef C2D_EXPORT_WRITERS

#include <memory>
#include <squish.h>

#include "core/graphics/image.h"
#include "core/graphics/texture.h"
#include "core/streams/arraystream.h"

c2d::ContentWriter::Result TextureWriter::write(DataStream& stream, const String& filename)
{
   c2d::Image image;
   if ( image.load(filename) )
   {
      stream.writeInt(image.getWidth());
      stream.writeInt(image.getHeight());
      //stream.writeInt(image.getFormat());

      if ( image.getFormat() != 4 )
      {
         int format;
         switch ( image.getFormat() )
         {
         case 1:
            format = Graphics::eFormat_Luminance;
            break;
         case 2:
            format = Graphics::eFormat_RG;
            break;
         case 3:
            format = Graphics::eFormat_RGBA;
            image.addAlphaChannel();
            break;
         default:
            throw std::runtime_error("Invalid texture format detected");
         }

         stream.writeInt(format);
         int size = image.getWidth() * image.getHeight() * image.getFormat();
         ArrayStream imagestream((const char*)image.getBytes(), size);
         stream.write(imagestream);
      }
      else
      {
         // https://technet.microsoft.com/en-us/subscriptions/downloads/ff471324(v=vs.85).aspx
         // libSquish is based on DX9 texture format specification
         // Based on the table for Texture2D the DXT5 maps to the new BC3 format

         stream.writeInt(4);
         int flags = squish::kDxt5 | squish::kColourClusterFit;
         int size = squish::GetStorageRequirements(image.getWidth(), image.getHeight(), flags);
         std::unique_ptr<unsigned char[]> data(new unsigned char[size]);
         squish::CompressImage(image.getBytes(), image.getWidth(), image.getHeight(), data.get(), flags);

         ArrayStream imagestream((char*)data.get(), size);
         stream.write(imagestream);
      }

      return eOk;
   }
   throw std::runtime_error("Could not load file");
}

#endif // C2D_EXPORT_WRITERS
