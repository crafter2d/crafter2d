
#include "D3D11RenderContext.h"

#include "core/graphics/indexbuffer.h"
#include "core/graphics/vertexbuffer.h"
#include "core/graphics/uniformbuffer.h"
#include "core/graphics/effect.h"

#include "d3dcodepath.h"
#include "d3dblendstate.h"
#include "d3dtexture.h"

#include "d3dxhelpers.h"

namespace Graphics
{
   ID3D11DeviceContext3& D3D11RenderContext::asContext(RenderContext& context)
   {
      return static_cast<D3D11RenderContext&>(context).getContext();
   }

   D3D11RenderContext::D3D11RenderContext(ID3D11DeviceContext3* pcontext, ID3D11RenderTargetView1* targetview, float dpi):
      RenderContext(),
      m_d3dContext(pcontext),
      m_d3dRenderTargetView(targetview)
   {
      DX::SafeAcquire(m_d3dContext);
      DX::SafeAcquire(m_d3dRenderTargetView);

      setDpi(dpi);
   }

   D3D11RenderContext::~D3D11RenderContext()
   {
      DX::SafeRelease(&m_d3dContext);
      DX::SafeRelease(&m_d3dRenderTargetView);
   }

   ID3D11DeviceContext3& D3D11RenderContext::getContext()
   { 
      return *m_d3dContext;
   }

   void D3D11RenderContext::setRenderTarget(RenderTarget& target)
   {
   }

   void D3D11RenderContext::setCodePath(CodePath& path)
   {
      static_cast<D3DCodePath&>(path).enable(*this);
   }

   void D3D11RenderContext::setBlendState(const BlendState& state)
   {
      static_cast<const D3DBlendState&>(state).enable(*m_d3dContext);
   }

   void D3D11RenderContext::setVertexBuffer(const VertexBuffer& buffer)
   {
      buffer.enable(*this);
   }

   void D3D11RenderContext::setIndexBuffer(const IndexBuffer& buffer)
   {
      buffer.enable(*this);
   }

   void D3D11RenderContext::setTexture(int stage, const Texture& texture)
   {
      texture.enable(*this, stage);
   }

   // drawing instructions
   void D3D11RenderContext::drawPoints(int start, int count)
   {
      m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
      m_d3dContext->Draw(count, start);
   }

   void D3D11RenderContext::drawTriangles(int start, int count)
   {
      m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      m_d3dContext->DrawIndexed(count, start, 0);
   }

   void D3D11RenderContext::drawTriangleFan(int start, int count)
   {
   }

   void D3D11RenderContext::drawTriangleStrip(int start, int count)
   {

   }

   void D3D11RenderContext::clear()
   {
      const Color& color = getClearColor();
      const float clear[4] = { color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha() };

      ID3D11RenderTargetView* const targets[1] = { m_d3dRenderTargetView };
      m_d3dContext->OMSetRenderTargets(1, targets, NULL);

      m_d3dContext->ClearRenderTargetView(m_d3dRenderTargetView, clear);
   }
}
