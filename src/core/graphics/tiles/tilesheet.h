
#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include <unordered_map>
#include <vector>

#include "core/core_base.h"
#include "core/graphics/texturecoordinate.h"
#include "core/string/string.h"

namespace Graphics
{
   class RenderContext;
   class Texture;
}

namespace c2d
{
   class CORE_API TileSheet
   {
   public:
      struct Tile
      {
         TextureCoordinate coord;
         bool rotated;
      };

      using Lookup = std::unordered_map<String, int>;
      using Tiles = std::vector<Tile>;

      TileSheet();

      void setTexture(Graphics::Texture* ptexture) {
         mpTexture = ptexture;
      }

      const TextureCoordinate& getCoordinate(int index) const {
         return mTiles[index].coord;
      }

      const Tile& getTile(int index) const {
         return mTiles[index];
      }

      void add(const String& name, const Tile& tile);

      int lookup(const String& name) const;

      void bind(Graphics::RenderContext& context) const;

   private:

      Graphics::Texture* mpTexture;
      Lookup             mLookup;
      Tiles              mTiles;
   };
}

#endif // SPRITE_SHEET_H

