
#ifndef GLYPH_VERTEX_DATA_H
#define GLYPH_VERTEX_DATA_H

#include "core/math/vertex.h"

namespace Graphics
{
   class GlyphVertexData
   {
   public:

      Vector mGlyphSize;
      float mGlyphAdvance;

      Vector mTexturePos;
      Vector mTextureDim;
   };
}

#endif // GLYPH_VERTEX_DATA_H
