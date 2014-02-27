
#include "worldwriter.h"

#include "core/streams/filereaderstream.h"
#include "core/vfs/stdiofile.h"

bool WorldWriter::write(DataStream& stream, const String& filename)
{
   StdioFile file;
   if ( file.open(filename, File::ERead | File::EBinary) )
   {
      String name;

      uint32_t len;
      FileReaderStream filestream(file);
      filestream.readUint32(len);
      const char* pdata = filestream.readBytes(len * 2);

      if ( name.isEmpty() )
      {
         int aap = 5;
      }
   }
   
   return true;
}
