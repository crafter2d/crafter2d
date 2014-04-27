
#include "d3dglyphprovider.h"

#include <stdio.h>
#include <string.h>

#include "core/graphics/text/glyph.h"
#include "core/math/size.h"

#include "d3dfont.h"

namespace Graphics
{
   D3DGlyphProvider::D3DGlyphProvider(ID2D1Device* pd2ddevice, IDWriteFactory* pdwfactory) :
      mpD2DDevice(pd2ddevice),
      mpD2DContext(NULL),
      mpBitmap(NULL),
      mpBrush(NULL),
      mpDWriteFactory(pdwfactory)
   {
      mpD2DDevice->AddRef();
      mpDWriteFactory->AddRef();
   }

   D3DGlyphProvider::~D3DGlyphProvider()
   {
      SafeRelease(&mpBrush);
      SafeRelease(&mpBitmap);
      SafeRelease(&mpD2DContext);
      SafeRelease(&mpD2DDevice);
      SafeRelease(&mpDWriteFactory);
   }

   bool D3DGlyphProvider::initialize(int width, int height)
   {
      HRESULT hr = mpD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &mpD2DContext);
      if ( FAILED(hr) )
      {
         return false;
      }

      mWidth = 384;
      if ( width > 0 && width <= 2048 )
         mWidth = width;

      mHeight = 384;
      if ( height > 0 && height <= 2048 )
         mHeight = height;

      D2D1_COLOR_F color = { 0, 0, 0, 1 };
      hr = mpD2DContext->CreateSolidColorBrush(color, &mpBrush);
      if ( FAILED(hr) )
      {
         return false;
      }

      float dpiX, dpiY;
      mpD2DContext->GetDpi(&dpiX, &dpiY);

      D2D1_SIZE_U size = { mWidth, mHeight };
      D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(
         DXGI_FORMAT_B8G8R8A8_UNORM,
         D2D1_ALPHA_MODE_IGNORE
      );

      // create the target bitmap that can be bound to the context
      D2D1_BITMAP_PROPERTIES targetProperties;
      targetProperties.pixelFormat = pixelFormat;
      targetProperties.dpiX = dpiX;
      targetProperties.dpiY = dpiY;

      hr = mpD2DContext->CreateBitmap(size, targetProperties, &mpTargetBitmap);
      if ( FAILED(hr) )
      {
         return false;
      }

      // create the mappeable bitmap to get the result
      D2D1_BITMAP_PROPERTIES1 mapProperties;
      ZeroMemory(&mapProperties, sizeof(mapProperties));
      mapProperties.pixelFormat = pixelFormat;
      mapProperties.dpiX = dpiX;
      mapProperties.dpiY = dpiY;
      mapProperties.bitmapOptions = D2D1_BITMAP_OPTIONS_CPU_READ | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;

      hr = mpD2DContext->CreateBitmap(size, NULL, 0, mapProperties, &mpBitmap);
      if ( FAILED(hr) )
      {
         return false;
      }
      return true;
   }

   void D3DGlyphProvider::initGlyphData(const DWRITE_FONT_METRICS& fontMetrics,
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
      outGlyphData.maxWidth = static_cast<LONG>(aw - r - l + 2.0f);
      outGlyphData.maxHeight = static_cast<LONG>(ah - b - t + 2.0f);
   }

   Glyph* D3DGlyphProvider::getGlyph(Font& font, uint16_t glyphindex, float emsize)
   {
      D3DFont& d3dfont = static_cast<D3DFont&>(font);

      DWRITE_FONT_METRICS fontmetrics;
      d3dfont.mpFontFace->GetMetrics(&fontmetrics);

      DWRITE_GLYPH_METRICS glyphMetrics;
      HRESULT hResult = d3dfont.mpFontFace->GetDesignGlyphMetrics(&glyphindex, 1, &glyphMetrics, FALSE);
      if ( FAILED(hResult) )
         return NULL;

      DWGlyphData glyphdata;
      initGlyphData(fontmetrics, glyphMetrics, emsize, glyphdata);

      float glyphAdvance = 0.0f;
      DWRITE_GLYPH_OFFSET glyphOffset = { 0.0f, 0.0f };

      DWRITE_GLYPH_RUN glyphrun;
      ZeroMemory(&glyphrun, sizeof(glyphrun));
      glyphrun.fontFace = d3dfont.mpFontFace;
      glyphrun.fontEmSize = emsize;
      glyphrun.glyphCount = 1;
      glyphrun.glyphIndices = &glyphindex;
      glyphrun.glyphAdvances = &glyphAdvance;
      glyphrun.glyphOffsets = &glyphOffset;

      D2D1_POINT_2F baseline = { 2.0f - glyphdata.offsetX, 2.0f - glyphdata.offsetY };

      mpD2DContext->SetTarget(mpTargetBitmap);
      mpD2DContext->BeginDraw();
      mpD2DContext->DrawGlyphRun(baseline, &glyphrun, mpBrush);
      mpD2DContext->EndDraw();
     
      // copy the glyph into the correct bitmap so we can extract it
      D2D1_MAPPED_RECT mapped;
      mpBitmap->CopyFromBitmap(NULL, mpTargetBitmap, NULL);
      HRESULT hr = mpBitmap->Map(D2D1_MAP_OPTIONS_READ, &mapped);
      if ( FAILED(hr) )
      {
         return NULL;
      }

      uint32_t size = mWidth * mHeight * 4;
      uint8_t* pdata = new uint8_t[size];
      memcpy(pdata, mapped.bits, size);

      mpBitmap->Unmap();

      Glyph* presult = new Glyph();
      presult->setPixels(pdata, mapped.pitch);
      presult->setSize(Size(glyphdata.maxWidth, glyphdata.maxHeight));

      return presult;
   }
}
