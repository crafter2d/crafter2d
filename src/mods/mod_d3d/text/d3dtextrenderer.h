
#ifndef D3D_TEXT_RENDERER_H
#define D3D_TEXT_RENDERER_H

#include <d2d1_1.h>
#include <dwrite_1.h>

class D3DTextRenderer : public IDWriteTextRenderer
{
public:
            D3DTextRenderer();
   virtual ~D3DTextRenderer();

 // operations
   HRESULT initialize(ID2D1DeviceContext* pcontext, int width, int height);

 // IUknown interface
   HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** pobject);
   ULONG   STDMETHODCALLTYPE AddRef();
   ULONG   STDMETHODCALLTYPE Release();

 // overrides
   virtual HRESULT STDMETHODCALLTYPE IsPixelSnappingDisabled(
        _In_opt_ void* clientDrawingContext,
        _Out_ BOOL* isDisabled) override;

   virtual HRESULT STDMETHODCALLTYPE GetCurrentTransform(
        _In_opt_ void* clientDrawingContext,
        _Out_ DWRITE_MATRIX* transform) override;

   virtual HRESULT STDMETHODCALLTYPE GetPixelsPerDip(
        _In_opt_ void* clientDrawingContext,
        _Out_ FLOAT* pixelsPerDip) override;

   virtual HRESULT STDMETHODCALLTYPE DrawGlyphRun(
      void* clientDrawingContext,
      FLOAT baselineOriginX,
      FLOAT baselineOriginY,
      DWRITE_MEASURING_MODE measuringMode,
      DWRITE_GLYPH_RUN const* glyphRun,
      DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
      IUnknown* clientDrawingEffect) override;

   virtual HRESULT STDMETHODCALLTYPE DrawUnderline(
      void* clientDrawingContext,
      FLOAT baselineOriginX,
      FLOAT baselineOriginY,
      DWRITE_UNDERLINE const* underline,
      IUnknown* clientDrawingEffect) override;

   virtual HRESULT STDMETHODCALLTYPE DrawStrikethrough(
      void* clientDrawingContext,
      FLOAT baselineOriginX,
      FLOAT baselineOriginY,
      DWRITE_STRIKETHROUGH const* strikethrough,
      IUnknown* clientDrawingEffect) override;

   virtual HRESULT STDMETHODCALLTYPE DrawInlineObject(
      void* clientDrawingContext,
      FLOAT originX,
      FLOAT originY,
      IDWriteInlineObject* inlineObject,
      BOOL isSideways,
      BOOL isRightToLeft,
      IUnknown* clientDrawingEffect) override;

private:
   struct DWGlyphData
   {
      FLOAT					offsetX;
      FLOAT					offsetY;
      int					maxWidth;
      int					maxHeight;
   };

   // helpers
   void initGlyphData(const DWRITE_FONT_METRICS& fontMetrics,
                      const DWRITE_GLYPH_METRICS& glyphMetrics,
                      float fontSize,
                      DWGlyphData& outGlyphData);

 // data
   ULONG                   mRefCount;
   ID2D1DeviceContext*     mpD2DContext;
   ID2D1SolidColorBrush*   mpBrush;
   ID2D1Bitmap*            mpTarget;
   ID2D1Bitmap1*           mpBitmap;
   D2D1_SIZE_U             mSize;
};

#endif // D3D_TEXT_RENDERER_H
