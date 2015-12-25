
#include "d3dfontcollection.h"

#include <vector>

#include "core/string/string.h"
#include "core/vfs/filesystem.h"

#include "../d3dxhelpers.h"

#include "d3dfont.h"
#include "d3dfontcollectionloader.h"

D3DFontCollection::D3DFontCollection():
   mpDWriteFactory(NULL),
   mpFontCollection(NULL),
   mFiles(NULL),
   mFileCount(0)
{
}

D3DFontCollection::~D3DFontCollection()
{
   DX::SafeRelease(&mpFontCollection);
   DX::SafeRelease(&mpDWriteFactory);
}

bool D3DFontCollection::initialize(IDWriteFactory* pdwfactory)
{
   mpDWriteFactory = pdwfactory;
   mpDWriteFactory->AddRef();

   std::vector<String> files;
   FileSystem::getInstance().find(UTEXT("*.ttf"), files, true);

   mFileCount = files.size();
   mFiles = new String[files.size()];

   for ( int index = 0; index < mFileCount; ++index )
   {
      mFiles[index] = files[index];
   }

   D3DFontCollectionLoader* ploader = &D3DFontCollectionLoader::getInstance();
   ploader->setCollection(*this);

   pdwfactory->RegisterFontCollectionLoader(ploader);

   HRESULT hr = pdwfactory->CreateCustomFontCollection(ploader, NULL, 0, &mpFontCollection);
   if ( FAILED(hr) )
   {
      return false;
   }

   return true;
}

Graphics::D3DFont* D3DFontCollection::createFont(const String& name)
{
   String filename = name + UTEXT(".ttf");
   IDWriteFontFile* pFontFile = NULL;
   HRESULT hr = mpDWriteFactory->CreateFontFileReference(filename.c_str(), NULL, &pFontFile);
   if ( FAILED(hr) )
   {
      return false;
   }

   Graphics::D3DFont* presult = NULL;
   IDWriteFontFace* pface = NULL;
   IDWriteFontFile* fontFileArray[] = { pFontFile };
   hr = mpDWriteFactory->CreateFontFace(DWRITE_FONT_FACE_TYPE_TRUETYPE,
      1, fontFileArray, 0, DWRITE_FONT_SIMULATIONS_NONE, &pface);
   if ( SUCCEEDED(hr) )
   {
      presult = new Graphics::D3DFont(*this, pface);
      presult->setFamilyName(name);
   }

   DX::SafeRelease(&pFontFile);

   return presult;
}

int D3DFontCollection::getFileCount() const
{
   return mFileCount;
}

const String& D3DFontCollection::getFile(int pos)
{
   return mFiles[pos];
}

IDWriteFontCollection* D3DFontCollection::getCustomFontCollection()
{
   return mpFontCollection;
}
