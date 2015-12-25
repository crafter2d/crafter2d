
#include "worldwriter.h"

#include "core/math/vector.h"
#include "core/streams/filereaderstream.h"
#include "core/vfs/stdiofile.h"

bool WorldWriter::write(DataStream& stream, const String& filename)
{
   StdioFile file;
   if ( file.open(filename, File::ERead | File::EBinary) )
   {
      FileReaderStream filestream(file);

      // read the header
      int version, layertype;
      filestream.readInt(version);
      String name = readString(filestream);
      String simulator = readString(filestream);
      filestream.readInt(layertype);

      stream << version << name << simulator << layertype;

      int layercount;
      filestream >> layercount;
      stream << layercount;
      for ( int index = 0; index < layercount; ++index )
      {
         int width, height;

         // read layer header
         String layername = readString(filestream);
         String effect = readString(filestream);
         String tileset = readString(filestream);
         filestream >> width >> height;

         int pos = effect.indexOf(L'.');
         if ( pos >= 0 )
         {
            effect = effect.subStr(0, pos);
         }
         pos = tileset.indexOf(L'.');
         if ( pos >= 0 )
         {
            tileset = tileset.subStr(0, pos);
         }

         stream << layername << effect << tileset;

         // read the field data
         int fieldwidth, fieldheight, datasize;
         filestream >> fieldwidth >> fieldheight >> datasize;

         uint8_t* pdata = new uint8_t[datasize];
         filestream.readBlob(pdata, datasize);

         stream << fieldwidth << fieldheight << datasize;
         stream.writeBlob(pdata, datasize);

         delete[] pdata;
      }

      // read the bounds
      int boundCount;
      filestream >> boundCount;
      stream << boundCount;
      for ( int index = 0; index < boundCount; ++index )
      {
         double leftx, lefty, rightx, righty;
         filestream >> leftx >> lefty >> rightx >> righty;

         Vector left((float)leftx, (float)lefty),
            right((float)rightx, (float)righty);
         stream << left << right;
      }
   }
   
   return true;
}

String WorldWriter::readString(DataStream& stream)
{
   String result;
   uint32_t bytes;
   stream.readUint32(bytes);
   if ( bytes != 0xFFFFFFFF )
   {
      int chars = bytes / sizeof(wchar_t);
      wchar_t* pdata = new wchar_t[chars+1];
      stream.readBlob(pdata, bytes);
      pdata[chars] = 0;

      result.assign(pdata, chars);
   }
   return result;
}
