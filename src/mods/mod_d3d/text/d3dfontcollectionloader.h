
#ifndef D3D_FONT_COLLECTION_LOADER_H
#define D3D_FONT_COLLECTION_LOADER_H

#include <dwrite_1.h>

namespace Graphics
{
   class D3DFontCollection;

   class D3DFontCollectionLoader : public IDWriteFontCollectionLoader
   {
   public:
      // singleton
      static D3DFontCollectionLoader& getInstance();

      // get/set
      void setCollection(D3DFontCollection& pcollection);

      // IUnknown interface
      HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** pobject);
      ULONG   STDMETHODCALLTYPE AddRef();
      ULONG   STDMETHODCALLTYPE Release();

      // overrides
      HRESULT STDMETHODCALLTYPE CreateEnumeratorFromKey(
         IDWriteFactory* factory,
         void const* collectionKey,                      // [collectionKeySize] in bytes
         UINT32 collectionKeySize,
         OUT IDWriteFontFileEnumerator** fontFileEnumerator) override;

   private:
      D3DFontCollectionLoader();

      // singleton
      static D3DFontCollectionLoader* spInstance;

      // data
      ULONG mRefCount;

      D3DFontCollection* mpCollection;
   };
}

#endif // D3D_FONT_COLLECTION_LOADER_H
