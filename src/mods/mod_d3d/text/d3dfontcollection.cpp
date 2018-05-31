
#include "d3dfontcollection.h"

#include <vector>

#include "core/string/string.h"
#include "core/vfs/filesystem.h"

#include "../d3dhelpers.h"

#include "d3dfont.h"
#include "d3dfontcollectionloader.h"
#include "d3dglyphprovider.h"

namespace Graphics
{
   D3DFontCollection::D3DFontCollection() :
      mpDWriteFactory(nullptr),
      mpFontCollection(nullptr),
      mFiles()
   {
   }

   D3DFontCollection::~D3DFontCollection()
   {
      SafeRelease(&mpFontCollection);
      SafeRelease(&mpDWriteFactory);
   }

   bool D3DFontCollection::initialize(IDWriteFactory* pdwfactory)
   {
      mpDWriteFactory = pdwfactory;
      mpDWriteFactory->AddRef();

      FileSystem::getInstance().find(UTEXT("*.ttf"), mFiles, true);
      
      D3DFontCollectionLoader* ploader = &D3DFontCollectionLoader::getInstance();
      ploader->setCollection(*this);

      pdwfactory->RegisterFontCollectionLoader(ploader);

      HRESULT hr = pdwfactory->CreateCustomFontCollection(ploader, nullptr, 0, &mpFontCollection);
      if (FAILED(hr))
      {
         return false;
      }

      return true;
   }

   D3DFont* D3DFontCollection::createFont(const String& name, D3DGlyphProvider* pprovider)
   {
      String filename = name + UTEXT(".ttf");
      IDWriteFontFile* pFontFile = nullptr;
      HRESULT hr = mpDWriteFactory->CreateFontFileReference(filename.c_str(), nullptr, &pFontFile);
      if (FAILED(hr))
      {
         return nullptr;
      }

      std::unique_ptr<D3DFont> result;
      IDWriteFontFace* pface = nullptr;
      IDWriteFontFile* fontFileArray[] = { pFontFile };
      hr = mpDWriteFactory->CreateFontFace(
         DWRITE_FONT_FACE_TYPE_TRUETYPE,
         1,
         fontFileArray,
         0,
         DWRITE_FONT_SIMULATIONS_NONE,
         &pface);
      if (SUCCEEDED(hr))
      {
         result = std::make_unique<D3DFont>(pface, pprovider);
         result->setFamilyName(name);

         pprovider->initialize(mpFontCollection, *result);
      }

      SafeRelease(&pFontFile);

      return result.release();
   }

   IDWriteFontCollection* D3DFontCollection::getCustomFontCollection()
   {
      return mpFontCollection;
   }
}
