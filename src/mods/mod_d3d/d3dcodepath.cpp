
#include "d3dcodepath.h"

#include "core/graphics/rendercontext.h"
#include "core/graphics/texture.h"
#include "core/graphics/vertexlayout.h"
#include "core/graphics/vertexlayoutelement.h"
#include "core/string/string.h"
#include "core/streams/filereaderstream.h"
#include "core/vfs/file.h"
#include "core/vfs/filesystem.h"
#include "core/vfs/memorybuffer.h"

#include "d3ddevice.h"
#include "d3duniformbuffer.h"

namespace Graphics
{

D3DCodePath::D3DCodePath(D3DDevice& device):
   CodePath(),
   mDevice(device),
   mpInputLayout(nullptr),
   mpVertexShader(nullptr),
   mpGeometryShader(nullptr),
   mpPixelShader(nullptr)
{
}

D3DCodePath::~D3DCodePath()
{
   release();
}

UniformBuffer* D3DCodePath::getUniformBuffer(const String& name) const
{
   D3DUniformBuffer* presult = new D3DUniformBuffer();
   return presult;
}

bool D3DCodePath::create(VertexLayout* playout, DataStream& vertexshader, DataStream& geometryshader, DataStream& pixelshader)
{
   setVertexLayout(playout);

   return loadVertexShader(vertexshader) && loadGeometryShader(geometryshader) && loadPixelShader(pixelshader);
}

bool D3DCodePath::loadVertexShader(DataStream& stream)
{
   HRESULT hr = mDevice.getDevice().CreateVertexShader(stream.getData(), stream.getDataSize(), nullptr, &mpVertexShader);
   if ( FAILED(hr) )
   {
      return false;
   }

   if ( !createInputLayout(stream) )
   {
      return false;
   }
      
   return true;
}

bool D3DCodePath::loadGeometryShader(DataStream& stream)
{
   if ( stream.getDataSize() > 0 )
   {
      HRESULT hr = mDevice.getDevice().CreateGeometryShader(stream.getData(), stream.getDataSize(), nullptr, &mpGeometryShader);
      if ( FAILED(hr) )
      {
         return false;
      }
   }
   return true;
}

bool D3DCodePath::loadPixelShader(DataStream& stream)
{
   if ( stream.getDataSize() > 0 )
   {
      HRESULT hr = mDevice.getDevice().CreatePixelShader(stream.getData(), stream.getDataSize(), nullptr, &mpPixelShader);
      if ( FAILED(hr) )
      {
         return false;
      }
   }

   return true;
}

bool D3DCodePath::createInputLayout(const DataStream& stream)
{
   const VertexLayout& layout = getVertexLayout();

   D3D11_INPUT_ELEMENT_DESC* pdescs = new D3D11_INPUT_ELEMENT_DESC[layout.getSize()];
   for ( int index = 0; index < layout.getSize(); ++index )
   {
      const VertexLayoutElement& element = layout[index];
      D3D11_INPUT_ELEMENT_DESC& desc = pdescs[index];

      memset(&desc, 0, sizeof(D3D11_INPUT_ELEMENT_DESC));
      desc.SemanticName = strdup(element.semantic.toUtf8().c_str());
      desc.Format = static_cast<DXGI_FORMAT>(element.type);
      desc.AlignedByteOffset = element.pos;
      desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
   }

   HRESULT hr = mDevice.getDevice().CreateInputLayout(pdescs, layout.getSize(), stream.getData(), stream.getDataSize(), &mpInputLayout);

   for ( int index = 0; index < layout.getSize(); ++index )
   {
      D3D11_INPUT_ELEMENT_DESC& desc = pdescs[index];
      delete[] desc.SemanticName;
   }
   delete[] pdescs;

   if ( FAILED(hr) )
   {
      return false;
   }

   return true;
}

void D3DCodePath::release()
{
   mpInputLayout->Release();
   mpInputLayout = nullptr;

   mpVertexShader->Release();
   mpVertexShader = nullptr;

   mpPixelShader->Release();
   mpPixelShader = nullptr;
}

void D3DCodePath::enable(RenderContext& context) const
{
   ID3D11DeviceContext& d3dcontext = D3DRenderContext::asContext(context);
   d3dcontext.IASetInputLayout(mpInputLayout);
   d3dcontext.VSSetShader(mpVertexShader, nullptr, 0);
   d3dcontext.GSSetShader(mpGeometryShader, nullptr, 0);
   d3dcontext.PSSetShader(mpPixelShader, nullptr, 0);
}

void D3DCodePath::disable(RenderContext& context) const
{
   if ( mpGeometryShader != nullptr )
   {
      ID3D11DeviceContext& d3dcontext = D3DRenderContext::asContext(context);
      d3dcontext.GSSetConstantBuffers(0, 0, nullptr);
   }
}

void D3DCodePath::bindTexture(RenderContext& context, int stage, const Texture& uniform)
{
   uniform.enable(context, stage);
}

void D3DCodePath::setConstantBuffer(RenderContext& context, int slot, const UniformBuffer& buffer)
{
   ID3D11DeviceContext& d3dcontext = D3DRenderContext::asContext(context);
   ID3D11Buffer* pbuffer = static_cast<const D3DUniformBuffer&>(buffer).getBuffer();

   d3dcontext.VSSetConstantBuffers(slot, 1, &pbuffer);
   if ( mpGeometryShader != nullptr )
   {
      d3dcontext.GSSetConstantBuffers(slot, 1, &pbuffer);
   }
}

} // namespace Graphics
