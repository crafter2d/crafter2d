
#include "tilesetwriter.h"

#include <vector>
#include <tinyxml.h>

#include "core/log/log.h"
#include "core/streams/datastream.h"
#include "core/world/tileset.h"

bool TileSetWriter::write(DataStream& stream, const String& filename)
{
   Log& log = Log::getInstance();

   std::string path = filename.toUtf8();
   TiXmlDocument doc(path);
   if ( !doc.LoadFile() )
   {
      log.error("TileSet.create: can not load '%s'", path.c_str());
		return false;
	}

   // find the tileset information element
   TiXmlElement* set = (TiXmlElement*)doc.FirstChild("tileset");
   if ( set == NULL )
   {
      log.error("TileSet.create: %s is not an tileset information file.", path.c_str());
		return false;
   }

   int tilecount, tilewidth, tileheight;
   set->QueryIntAttribute("count", &tilecount);
   if ( tilecount <= 0 )
   {
      log.error("TileSet.create: %s contains invalid tileset size.", path.c_str());
      return false;
   }

   set->QueryIntAttribute("width", &tilewidth);
   set->QueryIntAttribute("height", &tileheight);

   if ( tilewidth == 0 || tileheight == 0 )
   {
      log.error("TileSet.create: invalid tile dimensions (in file %s)", path.c_str());
      return false;
   }

   stream << tilecount << tilewidth << tileheight;

   const TiXmlElement* pmapelement = static_cast<const TiXmlElement*>(set->FirstChild("texture"));
   if ( pmapelement == NULL )
   {
      return false;
   }

   const char* pfile = pmapelement->Attribute("name");
   if ( pfile == NULL )
   {
      pfile = pmapelement->GetText();
      if ( pfile == NULL )
      {
         return false;
      }
   }

   String texturename = String::fromUtf8(pfile);
   int index = texturename.indexOf(L'.');
   if ( index >= 0 )
   {
      texturename = texturename.subStr(0, index);
   }
   stream << texturename;

   std::vector<TileInfo> infos;
      
   // load information about all tiles
   TiXmlElement* tile = (TiXmlElement*)set->FirstChild("tile");
   while ( tile )
   {
      int tileid;
      int anim_length = 0;
      float anim_speed = 0.0f;

      tile->QueryIntAttribute("id", &tileid);

      // see if the tile has an animation child
      TiXmlElement* anim = (TiXmlElement*)tile->FirstChild("animation");
      if ( anim == NULL)
      {
         // invalid file format
         return false;
      }

      if ( anim->QueryIntAttribute("length", &anim_length) != TIXML_SUCCESS
         || anim->QueryFloatAttribute("speed", &anim_speed) != TIXML_SUCCESS )
      {
         // invalid file format
         return false;
      }

      TileInfo info;
      info.anim_index = tileid; // lets use this as tile index
      info.anim_length = anim_length;
      info.anim_speed = anim_speed;
      infos.push_back(info);

      // get next tile info element
      tile = (TiXmlElement*)set->IterateChildren ("tile", tile);
   }

   stream << (int) infos.size();
   for ( std::size_t index = 0; index < infos.size(); ++index )
   {
      TileInfo& info = infos[index];
      stream << info.anim_index << info.anim_length << info.anim_speed;
   }

   return true;
}
