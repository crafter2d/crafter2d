
#include "d3dfontfileenumerator.h"

#include "../d3dxhelpers.h"
#include "d3dfontcollection.h"

D3DFontFileEnumerator::D3DFontFileEnumerator(IDWriteFactory* pdwfactory):
   mpDWFactory(pdwfactory),
   mpDWFile(NULL),
   mRefCount(0),
   mpCollection(NULL),
   mPos(0)
{
}

// - Get/set

void D3DFontFileEnumerator::setCollection(D3DFontCollection& collection)
{
   mpCollection = &collection;
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
      *pobject = NULL;
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
   if ( newCount == 0 )
   {
      delete this;
   }
   return newCount;
}

// - Overrides

HRESULT D3DFontFileEnumerator::MoveNext(OUT BOOL* hasCurrentFile)
{
   if ( mPos < mpCollection->getFileCount() )
   {
      const String& filename = mpCollection->getFile(mPos);

      HRESULT hr = mpDWFactory->CreateFontFileReference(filename.c_str(), NULL, &mpDWFile);
      if ( SUCCEEDED(hr) )
      {
         mPos++;

         *hasCurrentFile = TRUE;

         return S_OK;
      }
   }

   *hasCurrentFile = FALSE;
   return S_OK;
}

HRESULT D3DFontFileEnumerator::GetCurrentFontFile(OUT IDWriteFontFile** fontFile)
{
   *fontFile = DX::SafeAcquire(mpDWFile);

   return S_OK;
}
