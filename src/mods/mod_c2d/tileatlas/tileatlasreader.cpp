#include "tileatlasreader.h"

#include "core/content/contentmanager.h"
#include "core/graphics/device.h"
#include "core/graphics/rendercontext.h"
#include "core/graphics/texture.h"
#include "core/graphics/tiles/tileatlas.h"
#include "core/streams/datastream.h"

IContent * c2d::TileAtlasReader::read(DataStream & stream)
{
   auto atlas = std::make_unique<TileAtlas>();
   
   int nsheets = 0;
   stream.readInt(nsheets);
   atlas->reserve(nsheets);
   for ( int index = 0; index < nsheets; index++ )
   {
      c2d::TileSheet& sheet = atlas->emplace();

      String textureName;
      stream.readString(textureName);
      auto ptexture = getContentManager().loadContent<Graphics::Texture>(textureName);
      sheet.setTexture(ptexture);

      int nsprites = 0;
      stream >> nsprites;
      for ( int sidx = 0; sidx < nsprites; ++sidx )
      {
         String name;
         c2d::TileSheet::Tile tile;

         stream >> name >> tile.coord.left >> tile.coord.right >> tile.coord.top >> tile.coord.bottom >> tile.rotated;

         sheet.add(name, tile);
      }
   }

   return atlas.release();
}
