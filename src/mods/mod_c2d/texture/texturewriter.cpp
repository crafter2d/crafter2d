
#include "texturewriter.h"

#include "core/streams/arraystream.h"
#include "core/vfs/stdiofile.h"

bool TextureWriter::write(DataStream& stream, const String& filename)
{
   StdioFile file;

   if ( file.open(filename, File::EBinary | File::ERead) )
   {
      int datalen = file.size();

      char* pbuffer = new char[datalen];
      int actualsize = file.read(pbuffer, datalen);
      file.close();

      ArrayStream arraystream(pbuffer, actualsize);
      stream.write(arraystream);

      return true;
   }

   return false;
}
