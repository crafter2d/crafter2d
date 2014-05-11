
#include "d3dtextrenderer.h"

#include <math.h>

#include "core/defines.h"
#include "core/graphics/text/glyph.h"

#include "../d3dhelpers.h"

D3DTextRenderer::D3DTextRenderer():
   mRefCount(0),
   mpD2DContext(NULL),
   mpBrush(NULL),
   mpTarget(NULL),
   mpBitmap(NULL),
   mSize()
{
}

D3DTextRenderer::~D3DTextRenderer()
{
   SafeRelease(&mpBrush);
   SafeRelease(&mpTarget);
   SafeRelease(&mpBitmap);
   SafeRelease(&mpD2DContext);
}

// - Operations

HRESULT D3DTextRenderer::initialize(ID2D1DeviceContext* pcontext, int width, int height)
{
   mpD2DContext = pcontext;
   mpD2DContext->AddRef();

   mSize.width = width;
   mSize.height = height;

   float dpiX, dpiY;
   mpD2DContext->GetDpi(&dpiX, &dpiY);

   D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(
      DXGI_FORMAT_B8G8R8A8_UNORM,
      D2D1_ALPHA_MODE_IGNORE
      );

   // create the target bitmap that can be bound to the context
   D2D1_BITMAP_PROPERTIES1 targetProperties;
   ZeroMemory(&targetProperties, sizeof(targetProperties));
   targetProperties.pixelFormat = pixelFormat;
   targetProperties.dpiX = dpiX;
   targetProperties.dpiY = dpiY;
   targetProperties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET;

   HRESULT hr = mpD2DContext->CreateBitmap(mSize, NULL, 0 ,targetProperties, &mpTarget);
   if ( FAILED(hr) )
   {
      return hr;
   }

   // create the mappeable bitmap to get the result
   D2D1_BITMAP_PROPERTIES1 mapProperties;
   ZeroMemory(&mapProperties, sizeof(mapProperties));
   mapProperties.pixelFormat = pixelFormat;
   mapProperties.dpiX = dpiX;
   mapProperties.dpiY = dpiY;
   mapProperties.bitmapOptions = D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

   hr = mpD2DContext->CreateBitmap(mSize, NULL, 0, mapProperties, &mpBitmap);
   if ( FAILED(hr) )
   {
      return hr;
   }

   // create the brush used to draw the glyph
   D2D1_COLOR_F color = { 1, 1, 1, 1 };
   hr = mpD2DContext->CreateSolidColorBrush(color, &mpBrush);
   if ( FAILED(hr) )
   {
      return hr;
   }

   return S_OK;
}

void D3DTextRenderer::initGlyphData(const DWRITE_FONT_METRICS& fontMetrics,
                                    const DWRITE_GLYPH_METRICS& glyphMetrics,
                                    float fontSize,
                                    DWGlyphData& outGlyphData)
{
   // Calculate pixel-space coordinates
   FLOAT fscale = fontSize / static_cast<FLOAT>(fontMetrics.designUnitsPerEm);

   FLOAT l = static_cast<FLOAT>(glyphMetrics.leftSideBearing) * fscale;
   FLOAT t = static_cast<FLOAT>(glyphMetrics.topSideBearing) * fscale;

   FLOAT r = static_cast<FLOAT>(glyphMetrics.rightSideBearing) * fscale;
   FLOAT b = static_cast<FLOAT>(glyphMetrics.bottomSideBearing) * fscale;

   FLOAT v = static_cast<FLOAT>(glyphMetrics.verticalOriginY) * fscale;

   FLOAT aw = static_cast<FLOAT>(glyphMetrics.advanceWidth) * fscale;
   FLOAT ah = static_cast<FLOAT>(glyphMetrics.advanceHeight) * fscale;

   // Set up glyph data
   outGlyphData.offsetX = floor(l);
   outGlyphData.offsetY = floor(t) - floor(v);
   outGlyphData.maxWidth = static_cast<int>(aw - r - l + 2.0f);
   outGlyphData.maxHeight = static_cast<int>(ah - b - t + 2.0f);
}

// IUnknown interface
   
HRESULT D3DTextRenderer::QueryInterface(REFIID iid, void** pobject)
{
   if (iid == IID_IUnknown || iid == __uuidof(IDWriteTextRenderer))
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

ULONG D3DTextRenderer::AddRef()
{
   return InterlockedIncrement(&mRefCount);
}

ULONG D3DTextRenderer::Release()
{
   ULONG newCount = InterlockedDecrement(&mRefCount);
   if ( newCount == 0 )
   {
      delete this;
   }
   return newCount;
}

// - Overrides

HRESULT D3DTextRenderer::IsPixelSnappingDisabled(
        _In_opt_ void* clientDrawingContext,
        _Out_ BOOL* isDisabled)
{
   *isDisabled = FALSE;
   return S_OK;
}

HRESULT D3DTextRenderer::GetCurrentTransform(
        _In_opt_ void* clientDrawingContext,
        _Out_ DWRITE_MATRIX* transform)
{
   transform->dx = 0;
   transform->dy = 1;
   transform->m11 = 1.0f;
   transform->m12 = 0.0f;
   transform->m21 = 0.0f;
   transform->m22 = 1.0f;
   return S_OK;
}

HRESULT D3DTextRenderer::GetPixelsPerDip(
        _In_opt_ void* clientDrawingContext,
        _Out_ FLOAT* pixelsPerDip)
{
   *pixelsPerDip = 96.0f;
   return S_OK;
}

HRESULT D3DTextRenderer::DrawGlyphRun(void* clientDrawingContext,
                                      FLOAT baselineOriginX,
                                      FLOAT baselineOriginY,
                                      DWRITE_MEASURING_MODE measuringMode,
                                      DWRITE_GLYPH_RUN const* glyphRun,
                                      DWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription,
                                      IUnknown* clientDrawingEffect)
{
   ASSERT(glyphRun->glyphCount == 1);

   DWRITE_FONT_METRICS fontmetrics;
   glyphRun->fontFace->GetMetrics(&fontmetrics);

   DWRITE_GLYPH_METRICS glyphMetrics;
   HRESULT hResult = glyphRun->fontFace->GetDesignGlyphMetrics(&glyphRun->glyphIndices[0], 1, &glyphMetrics, FALSE);
   if ( FAILED(hResult) )
      return E_FAIL;

   DWGlyphData glyphdata;
   initGlyphData(fontmetrics, glyphMetrics, glyphRun->fontEmSize, glyphdata);

   D2D1_POINT_2F baseline = { 2.0f - glyphdata.offsetX, 2.0f - glyphdata.offsetY };
   
   mpD2DContext->SetTarget(mpTarget);
   mpD2DContext->SetTransform(D2D1::Matrix3x2F::Identity());
   mpD2DContext->BeginDraw();
   mpD2DContext->Clear(D2D1::ColorF(D2D1::ColorF::Black));
   mpD2DContext->DrawGlyphRun(baseline, glyphRun, mpBrush);
   mpD2DContext->EndDraw();
     
   // copy the glyph into the correct bitmap so we can extract it
   D2D1_MAPPED_RECT mapped;
   HRESULT hr = mpBitmap->CopyFromBitmap(NULL, mpTarget, NULL);
   if ( FAILED(hr) )
   {
      return hr;
   }

   hr = mpBitmap->Map(D2D1_MAP_OPTIONS_READ, &mapped);
   if ( FAILED(hr) )
   {
      return E_FAIL;
   }

   uint32_t size = glyphdata.maxHeight * glyphdata.maxWidth * sizeof(uint8_t);
   uint8_t* pdata = new uint8_t[size];

   const BYTE* psrc = mapped.bits;
   uint8_t* pdest = pdata;
   uint32_t destpitch = glyphdata.maxWidth * sizeof(uint8_t);

   for ( int y = 0; y < glyphdata.maxHeight; ++y )
   {
      for ( int x = 0; x < glyphdata.maxWidth; ++x )
      {
         pdest[x] = psrc[x * 4];
         if ( pdest[x] > 0 )
         {
            int aap = 5;
         }
      }

      psrc += mapped.pitch;
      pdest += destpitch;
   }

   Graphics::Glyph* pglyph = (Graphics::Glyph*)clientDrawingContext;
   pglyph->setSize(Size(static_cast<float>(glyphdata.maxWidth), static_cast<float>(glyphdata.maxHeight)));
   pglyph->setPixels(pdata, destpitch);
   pglyph->setAdvance(glyphRun->glyphAdvances[0]);

   mpBitmap->Unmap();

   return S_OK;
}

HRESULT D3DTextRenderer::DrawUnderline(void* clientDrawingContext,
                                       FLOAT baselineOriginX,
                                       FLOAT baselineOriginY,
                                       DWRITE_UNDERLINE const* underline,
                                       IUnknown* clientDrawingEffect)
{
   return E_NOTIMPL;
}

HRESULT D3DTextRenderer::DrawStrikethrough(void* clientDrawingContext,
                                           FLOAT baselineOriginX,
                                           FLOAT baselineOriginY,
                                           DWRITE_STRIKETHROUGH const* strikethrough,
                                           IUnknown* clientDrawingEffect)
{
   return E_NOTIMPL;
}

HRESULT D3DTextRenderer::DrawInlineObject(void* clientDrawingContext,
                                          FLOAT originX,
                                          FLOAT originY,
                                          IDWriteInlineObject* inlineObject,
                                          BOOL isSideways,
                                          BOOL isRightToLeft,
                                          IUnknown* clientDrawingEffect)
{
   return E_NOTIMPL;
}
