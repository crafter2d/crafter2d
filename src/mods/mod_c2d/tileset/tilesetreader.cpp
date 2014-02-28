
#include "tilesetreader.h"

#include "core/content/contentmanager.h"
#include "core/graphics/texture.h"
#include "core/streams/datastream.h"
#include "core/world/tileset.h"

IContent* TileSetReader::read(DataStream& stream)
{
   TileSet* presult = new TileSet();

   int tilecount, tilewidth, tileheight;
   String texturename;

   stream >> tilecount >> tilewidth >> tileheight >> texturename;
   presult->setTileCount(tilecount);
   presult->setTileWidth(tilewidth);
   presult->setTileHeight(tileheight);

   Graphics::Texture* ptexture = getContentManager().loadContent<Graphics::Texture>(texturename);
   presult->setTexture(ptexture);

   int tileanimations;
   stream >> tileanimations;
   for ( int index = 0; index < tileanimations; ++index )
   {
      int tileid;
      stream >> tileid;

      TileInfo& info = (*presult)[tileid];
      stream >> info.anim_length >> info.anim_speed;
   }

   return presult;
}
