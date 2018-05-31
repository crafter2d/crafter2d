
#include "d3dfontfileenumerator.h"

#include "../d3dhelpers.h"
#include "d3dfontcollection.h"

namespace Graphics
{
   D3DFontFileEnumerator::D3DFontFileEnumerator(IDWriteFactory* pdwfactory) :
      mpDWFactory(pdwfactory),
      mpDWFile(nullptr),
      mRefCount(0),
      mpCollection(nullptr),
      mIterator()
   {
   }

   // - Get/set

   void D3DFontFileEnumerator::setCollection(D3DFontCollection& collection)
   {
      mpCollection = &collection;
      mIterator = mpCollection->getFiles().begin();
   }


   // IUnknown interface

   HRESULT D3DFontFileEnumerator::QueryInterface(REFIID iid, void** pobject)
   {
      if (iid == IID_IUnknown || iid == __uuidof(IDWriteFontCollectionLoader))
      {
         *pobject = this;
         AddRef();
         return S_OK;
      }
      else
      {
         *pobject = nullptr;
         return E_NOINTERFACE;
      }
   }

   ULONG D3DFontFileEnumerator::AddRef()
   {
      return InterlockedIncrement(&mRefCount);
   }

   ULONG D3DFontFileEnumerator::Release()
   {
      ULONG newCount = InterlockedDecrement(&mRefCount);
      if (newCount == 0)
      {
         delete this;
      }
      return newCount;
   }

   // - Overrides

   HRESULT D3DFontFileEnumerator::MoveNext(OUT BOOL* hasCurrentFile)
   {
      if ( mIterator != mpCollection->getFiles().end() )
      {
         const String& filename = *mIterator;

         HRESULT hr = mpDWFactory->CreateFontFileReference(filename.c_str(), nullptr, &mpDWFile);
         if (SUCCEEDED(hr))
         {
            mIterator++;

            *hasCurrentFile = TRUE;

            return S_OK;
         }
      }

      *hasCurrentFile = FALSE;
      return S_OK;
   }

   HRESULT D3DFontFileEnumerator::GetCurrentFontFile(OUT IDWriteFontFile** fontFile)
   {
      *fontFile = SafeAcquire(mpDWFile);

      return S_OK;
   }
}
