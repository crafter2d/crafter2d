
#include "d3dcodepath.h"

#include "core/graphics/rendercontext.h"
#include "core/graphics/texture.h"
#include "core/graphics/vertexlayout.h"
#include "core/graphics/vertexlayoutelement.h"
#include "core/smartptr/autoptr.h"
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
   mpInputLayout(NULL),
   mpVertexShader(NULL),
   mpPixelShader(NULL)
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

bool D3DCodePath::create(VertexLayout* playout, DataStream& vertexshader, DataStream& pixelshader)
{
   setVertexLayout(playout);

   return loadVertexProgram(vertexshader) && loadPixelProgram(pixelshader);
}

bool D3DCodePath::loadVertexProgram(DataStream& stream)
{
   HRESULT hr = mDevice.getDevice().CreateVertexShader(stream.getData(), stream.getDataSize(), NULL, &mpVertexShader);
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

bool D3DCodePath::loadPixelProgram(DataStream& stream)
{
   HRESULT hr = mDevice.getDevice().CreatePixelShader(stream.getData(), stream.getDataSize(), NULL, &mpPixelShader);
   if ( FAILED(hr) )
   {
      return false;
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
      desc.Format = DXGI_FORMAT_R32G32_FLOAT;
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
   mpInputLayout = NULL;

   mpVertexShader->Release();
   mpVertexShader = NULL;

   mpPixelShader->Release();
   mpPixelShader = NULL;
}

void D3DCodePath::enable(RenderContext& context) const
{
   ID3D11DeviceContext& d3dcontext = D3DRenderContext::asContext(context);
   d3dcontext.IASetInputLayout(mpInputLayout);
   d3dcontext.VSSetShader(mpVertexShader, NULL, 0);
   d3dcontext.PSSetShader(mpPixelShader, NULL, 0);
}

void D3DCodePath::disable(RenderContext& context) const
{
   
}

bool D3DCodePath::bindTexture(RenderContext& context, int stage, const Texture& uniform)
{
   uniform.enable(context, stage);
   return true;
}

} // namespace Graphics
