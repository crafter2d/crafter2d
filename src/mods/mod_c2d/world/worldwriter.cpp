
#include "worldwriter.h"

#include <memory>
#include <stdexcept>

#include "xml/xmlparser/xmlparser.h"
#include "xml/xmlparser/xmlparts.h"

#include "core/defines.h"
#include "core/math/vector.h"
#include "core/streams/filereaderstream.h"
#include "core/vfs/stdiofile.h"

using namespace c2d::xml;

ELEMENT_DEF(entity)
   ATTRIBUTE(std::string, name)
   ATTRIBUTE(float, x)
   ATTRIBUTE(float, y)
   ATTRIBUTE(float, scale)
   ATTRIBUTE(int, sortorder)
ELEMENT_IMP(entity)
   ATTRIBUTE_INIT(name, Attribute::eMandatory, Attribute::eString, "")
   ATTRIBUTE_INIT(x, Attribute::eMandatory, Attribute::eFloat, 0.0f)
   ATTRIBUTE_INIT(y, Attribute::eMandatory, Attribute::eFloat, 0.0f)
   ATTRIBUTE_INIT(scale, Attribute::eMandatory, Attribute::eFloat, 1.0f)
   ATTRIBUTE_INIT(sortorder, Attribute::eMandatory, Attribute::eInt, 0)
ELEMENT_END

ELEMENT_DEF(entities)
   CHILD(entity)
ELEMENT_IMP(entities)
   CHILD_INIT(entity, Child::eZeroOrAny)
ELEMENT_END

ELEMENT_DEF(worldinfo)
   CHILD(entities)
ELEMENT_IMP(worldinfo)
   CHILD_INIT(entities, Child::eZeroOrOne)
ELEMENT_END

c2d::ContentWriter::Result WorldWriter::write(DataStream& stream, const String& filename)
{
   if ( File::extension(filename) == UTEXT("xml") ) // processed by the world file
      return eOkNoStore;

   processWorldFile(stream, filename);
   processWorldInfo(stream, filename);
   
   return eOk;
}

void WorldWriter::processWorldFile(DataStream& stream, const String& filename)
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
         uint32_t flags;

         // read layer header
         String layername = readString(filestream);
         String effect = readString(filestream);
         String tileset = readString(filestream);
         filestream >> width >> height >> flags;

         std::size_t pos = effect.indexOf(L'.');
         if ( pos != String::npos )
         {
            effect = effect.subStr(0, pos);
         }
         pos = tileset.indexOf(L'.');
         if ( pos != String::npos )
         {
            tileset = tileset.subStr(0, pos);
         }
            
         stream << layername << effect << tileset << flags;

         if ( (flags & 1) == 1 )
         {
            // read the field data
            int fieldwidth, fieldheight, datasize;
            filestream >> fieldwidth >> fieldheight >> datasize;

            uint8_t* pdata = new uint8_t[datasize];
            filestream.readBlob(pdata, datasize);

            stream << fieldwidth << fieldheight << datasize;
            stream.writeBlob(pdata, datasize);

            delete[] pdata;
         }
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
}

void WorldWriter::processWorldInfo(DataStream& stream, const String& filename)
{
   size_t pos = filename.lastIndexOf(L'.');
   ASSERT(pos != String::npos);
   String xmlfile = filename.subStr(0, pos) + UTEXT(".xml");

   worldinfo info;
   XmlParser parser;
   if ( !parser.parse(info, xmlfile) )
      throw std::runtime_error("Could not process world info file");

   stream << !info.entitiess.empty();
   if ( info.entitiess.size() > 0 )
   {
      auto& ents = info.entitiess[0];
      stream << (uint32_t)ents.entitys.size();
      for ( auto& entity : ents.entitys )
      {
         String name(entity.name);
         name = name.left(name.length() - 4);

         stream << name << entity.x << entity.y << entity.scale << entity.sortorder;
      }
   }
}

String WorldWriter::readString(DataStream& stream)
{
   String result;
   uint32_t bytes;
   stream.readUint32(bytes);
   if ( bytes != 0xFFFFFFFF )
   {
      int chars = bytes / sizeof(wchar_t);
      std::unique_ptr<wchar_t[]> data(new wchar_t[chars+1]);
      stream.readBlob(data.get(), bytes);
      data[chars] = 0;

      result.assign(data.get(), chars);
   }
   return result;
}
