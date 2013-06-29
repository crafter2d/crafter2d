
#include "d3drendercontext.h"

namespace Graphics
{

D3DRenderContext::D3DRenderContext(ID3D11DeviceContext* pcontext):
   RenderContext(),
   mpContext(pcontext)
{
}

// - State

void D3DRenderContext::setBlendState(const BlendState& state)
{
}

void D3DRenderContext::setEffect(const Effect& effect)
{
}

void D3DRenderContext::setVertexBuffer(const VertexBuffer& buffer)
{
}

void D3DRenderContext::setIndexBuffer(const IndexBuffer& buffer)
{
}

// - Matrix operations

void D3DRenderContext::setOrthoProjection()
{
}

void D3DRenderContext::setObjectMatrix(const XForm& matrix)
{
}

void D3DRenderContext::setWorldMatrix(const XForm& matrix)
{
}

// - Drawing

void D3DRenderContext::clear()
{
}

void D3DRenderContext::drawTriangles(int start, int count)
{
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
