
#include "tilesetreader.h"

#include "core/content/contentmanager.h"
#include "core/graphics/device.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/texturecoordinate.h"
#include "core/streams/datastream.h"
#include "core/world/tileset.h"

IContent* TileSetReader::read(DataStream& stream)
{
   int tilecount, tilewidth, tileheight;
   stream >> tilewidth >> tileheight >> tilecount;
   
   auto& atlas = getGraphicsDevice().getContext().getSpriteAtlas();

   std::vector<TileInfo> infos;
   infos.resize(tilecount);
   for ( auto& info : infos )
   {
      String name;
      float offsetx, offsety;
      stream >> name >> offsetx >> offsety;
      int tileindex = atlas.lookup(name);

      auto& tile = atlas.getTile(tileindex);

      info.coords = tile.coord;
      info.offset.set(offsetx, offsety);
      info.sheet = tileindex >> 16;
      info.flag = tile.rotated ? TileRotated : 0;
   }

   TileSet* presult = new TileSet(tilewidth, tileheight);
   presult->setTileInfos(std::move(infos));

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
