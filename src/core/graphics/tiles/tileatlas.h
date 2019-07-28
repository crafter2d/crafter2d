#ifndef SPRITE_ATLAS_H
#define SPRITE_ATLAS_H

#include <vector>

#include "core/core_base.h"
#include "core/content/content.h"

#include "tilesheet.h"

namespace Graphics
{
   class RenderContext;
}

namespace c2d
{
   class TextureCoordinate;

   class CORE_API TileAtlas : public IContent
   {
   public:
      TileAtlas();

      int lookup(const String& name) const;

      const TextureCoordinate& getCoordinate(int index) const {
         return mSheets[index >> 16].getCoordinate(index & 0xffff);
      }

      const TileSheet::Tile& getTile(int index) const {
         return mSheets[index >> 16].getTile(index & 0xffff);
      }

      TileSheet& emplace();

      void reserve(int count);

      void bind(Graphics::RenderContext& context, int sheet) {
         mSheets[sheet].bind(context);
      }

   private:
      using Sheets = std::vector<TileSheet>;

      Sheets mSheets;
   };
}

#endif // SPRITE_ATLAS_H

