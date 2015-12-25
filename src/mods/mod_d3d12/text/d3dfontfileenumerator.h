
#ifndef D3D_FONT_FILE_ENUMERATOR_H
#define D3D_FONT_FILE_ENUMERATOR_H

#include <dwrite_1.h>

class D3DFontCollection;

class D3DFontFileEnumerator : public IDWriteFontFileEnumerator
{
public:
   D3DFontFileEnumerator(IDWriteFactory* pdwfactory);

 // get/set
   void setCollection(D3DFontCollection& collection);

 // IUnknown interface
   HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** pobject);
   ULONG   STDMETHODCALLTYPE AddRef();
   ULONG   STDMETHODCALLTYPE Release();

 // overrides
   virtual HRESULT STDMETHODCALLTYPE MoveNext(OUT BOOL* hasCurrentFile);
   virtual HRESULT STDMETHODCALLTYPE GetCurrentFontFile(OUT IDWriteFontFile** fontFile);

private:

  // data
   IDWriteFactory*    mpDWFactory;
   IDWriteFontFile*   mpDWFile;
   ULONG              mRefCount;
   D3DFontCollection* mpCollection;
   int                mPos;
};

#endif // D3D_FONT_FILE_ENUMERATOR_H
