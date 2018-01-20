
#ifndef D3D_FONT_COLLECTION_H
#define D3D_FONT_COLLECTION_H

#include <vector>
#include <dwrite_1.h>

#include "core/string/string.h"

namespace Graphics
{
   class D3DFont;
   class D3DGlyphProvider;

   class D3DFontCollection
   {
   public:
      using Files = std::vector<String>;

      D3DFontCollection();
      ~D3DFontCollection();

      bool initialize(IDWriteFactory* pdwfactory);

    // get/set
      const Files& getFiles() const {
         return mFiles;
      }

      IDWriteFontCollection* getCustomFontCollection();

    // operations
      D3DFont* createFont(const String& name, D3DGlyphProvider* pprovider);

   private:

    // data
      IDWriteFactory*        mpDWriteFactory;
      IDWriteFontCollection* mpFontCollection;
      Files                  mFiles;
   };
}

#endif // D3D_FONT_COLLECTION_H
