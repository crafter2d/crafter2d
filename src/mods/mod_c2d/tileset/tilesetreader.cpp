
#include "tilesetreader.h"

#include "core/content/contentmanager.h"
#include "core/graphics/device.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/texturecoordinate.h"
#include "core/streams/datastream.h"
#include "core/world/tileset.h"

IContent* TileSetReader::read(DataStream& stream)
{
   TileSet* presult = new TileSet();

   int tilecount, tilewidth, tileheight;
   
   stream >> tilewidth >> tileheight >> tilecount;
   presult->setTileWidth(tilewidth);
   presult->setTileHeight(tileheight);
   presult->setTileCount(tilecount);
   
   auto& atlas = getGraphicsDevice().getContext().getSpriteAtlas();

   for ( int index = 0; index < tilecount; ++index )
   {
      String name;
      stream >> name;
      int tileindex = atlas.lookup(name);

      TileInfo& info = (*presult)[index];
      info.coords = atlas.getCoordinate(tileindex);
      info.sheet = tileindex >> 16;
      info.flag = 0;
   }
   /*
   int tileanimations;
   stream >> tileanimations;
   for ( int index = 0; index < tileanimations; ++index )
   {
      int tileid;
      stream >> tileid;

      TileInfo& info = (*presult)[tileid];
      stream >> info.anim_length >> info.anim_speed;
   }
   */
   return presult;
}
