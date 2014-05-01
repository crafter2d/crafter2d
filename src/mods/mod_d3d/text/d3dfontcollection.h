
#ifndef D3D_FONT_COLLECTION_H
#define D3D_FONT_COLLECTION_H

#include <dwrite_1.h>

#include "core/string/string.h"

namespace Graphics
{
   class D3DFont;
}

class D3DFontCollection
{
public:
   D3DFontCollection();
   ~D3DFontCollection();

   bool initialize(IDWriteFactory* pdwfactory);

 // get/set
   int           getFileCount() const;
   const String& getFile(int pos);

   IDWriteFontCollection* getCustomFontCollection();

 // operations
   Graphics::D3DFont* createFont(const String& name);

private:

 // data
   IDWriteFactory*        mpDWriteFactory;
   IDWriteFontCollection* mpFontCollection;
   String*                mFiles;
   int                    mFileCount;
};

#endif // D3D_FONT_COLLECTION_H
