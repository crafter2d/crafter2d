
#include "d3dtextrenderer.h"

#include <math.h>

#include "core/defines.h"
#include "core/graphics/text/glyph.h"

#include "../d3dxhelpers.h"

D3DTextRenderer::D3DTextRenderer():
   mRefCount(0),
   mpD2DFactory(NULL),
   mpD2DContext(NULL),
   mpBrush(NULL),
   mpTarget(NULL),
   mpBitmap(NULL),
   mSize()
{
}

D3DTextRenderer::~D3DTextRenderer()
{
   DX::SafeRelease(&mpBrush);
   DX::SafeRelease(&mpTarget);
   DX::SafeRelease(&mpBitmap);
   DX::SafeRelease(&mpD2DContext);
}

// - Operations

HRESULT D3DTextRenderer::initialize(ID2D1Factory1* pfactory, ID2D1DeviceContext* pcontext, int width, int height)
{
   mpD2DFactory = pfactory;
   mpD2DFactory->AddRef();

   mpD2DContext = pcontext;
   mpD2DContext->AddRef();

   mSize.width = width;
   mSize.height = height;

   float dpiX = 96, dpiY = 96;
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
   

   float dpiX, dpiY;
   mpD2DContext->GetDpi(&dpiX, &dpiY);
   float dip = dpiX / 96.0f;
   float invdip = 1.0f / dip;

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
   outGlyphData.offsetX = floorf(l * invdip);
   outGlyphData.offsetY = floorf((t * invdip) - v);
   outGlyphData.advance = static_cast<int>(ceilf(aw * dip));
   outGlyphData.maxWidth = static_cast<int>(ceilf((aw - r - l) * dip));
   outGlyphData.maxHeight = static_cast<int>(ceilf((ah - b - t) * dip));
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
   float dpiX, dpiY;
   mpD2DContext->GetDpi(&dpiX, &dpiY);
   *pixelsPerDip = dpiX / 96.0f;
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

   ID2D1PathGeometry* pPathGeometry = NULL;
   HRESULT hr = mpD2DFactory->CreatePathGeometry(&pPathGeometry);

   // Write to the path geometry using the geometry sink.
   ID2D1GeometrySink* pSink = NULL;
   if ( SUCCEEDED(hr) )
   {
      hr = pPathGeometry->Open(&pSink);
   }

   if ( SUCCEEDED(hr) )
   {
      hr = glyphRun->fontFace->GetGlyphRunOutline(
            glyphRun->fontEmSize,
            glyphRun->glyphIndices,
            glyphRun->glyphAdvances,
            glyphRun->glyphOffsets,
            glyphRun->glyphCount,
            glyphRun->isSideways,
            glyphRun->bidiLevel % 2,
            pSink
            );
   }

   // Close the geometry sink
   if ( SUCCEEDED(hr) )
   {
      hr = pSink->Close();
   }

   // Initialize a matrix to translate the origin of the glyph run.
   D2D1::Matrix3x2F const matrix = D2D1::Matrix3x2F(
      1.0f, 0.0f,
      0.0f, 1.0f,
      baselineOriginX, baselineOriginY
      );

   D2D1_RECT_F bounds;
   pPathGeometry->GetBounds(matrix, &bounds);

   // Create the transformed geometry
   ID2D1TransformedGeometry* pTransformedGeometry = NULL;
   if ( SUCCEEDED(hr) )
   {
      hr = mpD2DFactory->CreateTransformedGeometry(
         pPathGeometry,
         &matrix,
         &pTransformedGeometry
         );
   }

   mpD2DContext->SetTarget(mpTarget);
   mpD2DContext->SetTransform(D2D1::Matrix3x2F::Identity());
   mpD2DContext->BeginDraw();
   mpD2DContext->Clear(D2D1::ColorF(D2D1::ColorF::Black));
   mpD2DContext->FillGeometry(pTransformedGeometry, mpBrush);
   mpD2DContext->EndDraw();

   DX::SafeRelease(&pTransformedGeometry);
   DX::SafeRelease(&pPathGeometry);
   DX::SafeRelease(&pSink);


   DWGlyphData glyphdata;
   initGlyphData(fontmetrics, glyphMetrics, glyphRun->fontEmSize, glyphdata);
     
   // copy the glyph into the correct bitmap so we can extract it
   D2D1_MAPPED_RECT mapped;
   hr = mpBitmap->CopyFromBitmap(NULL, mpTarget, NULL);
   if ( FAILED(hr) )
   {
      return hr;
   }

   hr = mpBitmap->Map(D2D1_MAP_OPTIONS_READ, &mapped);
   if ( FAILED(hr) )
   {
      return E_FAIL;
   }

   float dpiX, dpiY;
   mpD2DContext->GetDpi(&dpiX, &dpiY);
   float dip = dpiX / 96.0f;
   float invdip = 1.0f / dip;

   int left = floorf(bounds.left * invdip);
   int top = floorf(bounds.top * invdip);
   int bottom = ceilf(bounds.bottom * dip);
   int right = ceilf(bounds.right * dip);

   glyphdata.maxWidth = right - left;
   glyphdata.maxHeight = bottom - top;

   uint32_t size = glyphdata.maxHeight * glyphdata.maxWidth * sizeof(uint8_t);
   uint8_t* pdata = new uint8_t[size];

   uint32_t destpitch = glyphdata.maxWidth * sizeof(uint8_t);
   uint8_t* pdest = pdata;

   const BYTE* psrc = mapped.bits;

   for ( int srcy = top; srcy < bottom; ++srcy )
   {
      for ( int srcx = left; srcx < right; ++srcx, ++pdest )
      {
         *pdest = psrc[srcy * mapped.pitch + (srcx + 2) * 4];
      }
   }

   Graphics::Glyph* pglyph = (Graphics::Glyph*)clientDrawingContext;
   pglyph->setSize(Size(static_cast<float>(glyphdata.maxWidth), static_cast<float>(glyphdata.maxHeight)));
   pglyph->setPixels(pdata, destpitch);
   pglyph->setAdvance(glyphdata.advance);
   pglyph->setBaseLine(baselineOriginX);

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
