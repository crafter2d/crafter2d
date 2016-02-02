
#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include <vector>

#include "core/core_base.h"
#include "core/graphics/texturecoordinate.h"

namespace c2d
{
   class CORE_API SpriteSheet
   {
   public:
      using TexCoords = std::vector<Graphics::TextureCoordinate>;

      SpriteSheet();

   private:

      TexCoords mTexCoords;
   };
}

#endif // SPRITE_SHEET_H

