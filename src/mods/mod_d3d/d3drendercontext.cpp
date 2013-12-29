
#include "d3drendercontext.h"

#include "core/graphics/indexbuffer.h"
#include "core/graphics/vertexbuffer.h"
#include "core/graphics/uniformbuffer.h"
#include "core/graphics/effect.h"

#include "texture/d3dtexture.h"
#include "d3dcodepath.h"
#include "d3dblendstate.h"

namespace Graphics
{

ID3D11DeviceContext& D3DRenderContext::asContext(RenderContext& context)
{
   return *static_cast<D3DRenderContext&>(context).mpContext;
}

D3DRenderContext::D3DRenderContext(ID3D11DeviceContext* pcontext, ID3D11RenderTargetView* targetview):
   RenderContext(),
   mpContext(pcontext),
   mpRenderTargetView(targetview)
{
}

// - Get/set

ID3D11DeviceContext& D3DRenderContext::getContext()
{
   return *mpContext;
}

// - State

void D3DRenderContext::setRenderTarget(RenderTarget& target)
{

}

void D3DRenderContext::setCodePath(CodePath& path)
{
   static_cast<D3DCodePath&>(path).enable(*this);
}

void D3DRenderContext::setBlendState(const BlendState& state)
{
   static_cast<const D3DBlendState&>(state).enable(*mpContext);
}

void D3DRenderContext::setVertexBuffer(const VertexBuffer& buffer)
{
   buffer.enable(*this);
}

void D3DRenderContext::setIndexBuffer(const IndexBuffer& buffer)
{
   buffer.enable(*this);
}

void D3DRenderContext::setUniformBuffer(const UniformBuffer& buffer)
{
   buffer.enable(*this);
}

void D3DRenderContext::setTexture(int stage, const Texture& texture)
{
   texture.enable(*this, stage);
}

// - Drawing

void D3DRenderContext::clear()
{
   const Color& color = getClearColor();
   const float clear[4] = { color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha() };

   mpContext->ClearRenderTargetView(mpRenderTargetView, clear);
}

void D3DRenderContext::drawTriangles(int start, int count)
{
   mpContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
   mpContext->DrawIndexed(count, start, 0);

   // ID3D11RasterizerState state;
}

void D3DRenderContext::drawTriangleFan(int start, int count)
{
}

void D3DRenderContext::drawTriangleStrip(int start, int count)
{
}

// - Notifications

void D3DRenderContext::onViewportChanged(const Viewport& viewport)
{
}

} // namespace Graphics
