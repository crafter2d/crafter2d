
#include "d3dfontcollectionloader.h"

#include "../d3dhelpers.h"

#include "d3dfontfileenumerator.h"

namespace Graphics
{
   D3DFontCollectionLoader* D3DFontCollectionLoader::spInstance(new D3DFontCollectionLoader());

   D3DFontCollectionLoader& D3DFontCollectionLoader::getInstance()
   {
      return *spInstance;
   }

   D3DFontCollectionLoader::D3DFontCollectionLoader() :
      mRefCount(0)
   {
   }

   // - Get/set

   void D3DFontCollectionLoader::setCollection(D3DFontCollection& pcollection)
   {
      mpCollection = &pcollection;
   }

   // IUnknown interface

   HRESULT D3DFontCollectionLoader::QueryInterface(REFIID iid, void** pobject)
   {
      if (iid == IID_IUnknown || iid == __uuidof(IDWriteFontCollectionLoader))
      {
         *pobject = this;
         AddRef();
         return S_OK;
      }
      else
      {
         *pobject = NULL;
         return E_NOINTERFACE;
      }
   }

   ULONG D3DFontCollectionLoader::AddRef()
   {
      return InterlockedIncrement(&mRefCount);
   }

   ULONG D3DFontCollectionLoader::Release()
   {
      ULONG newCount = InterlockedDecrement(&mRefCount);
      if (newCount == 0)
      {
         delete this;
      }
      return newCount;
   }

   // - overrides

   HRESULT D3DFontCollectionLoader::CreateEnumeratorFromKey(
      IDWriteFactory* factory,
      void const* collectionKey,
      UINT32 collectionKeySize,
      OUT IDWriteFontFileEnumerator** fontFileEnumerator)
   {

      D3DFontFileEnumerator* penumerator = new D3DFontFileEnumerator(factory);
      if (penumerator == NULL)
      {
         return E_OUTOFMEMORY;
      }

      penumerator->setCollection(*mpCollection);

      *fontFileEnumerator = SafeAcquire(penumerator);

      return S_OK;
   }
}
