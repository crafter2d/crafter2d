
#ifndef D3D_CODE_PATH_H
#define D3D_CODE_PATH_H

#include "core/graphics/codepath.h"

class DataStream;

struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11GeometryShader;
struct ID3D11PixelShader;

namespace Graphics
{
   class D3DDevice;

   class D3DCodePath : public CodePath
   {
   public:
      explicit D3DCodePath(D3DDevice& device);
      virtual ~D3DCodePath();

    // query
      virtual UniformBuffer* getUniformBuffer(const String& name) const override;

      virtual bool      create(VertexLayout* playout, DataStream& vertexshader, DataStream& geometryshader, DataStream& pixelshader) override;
      virtual void      release() override;

      virtual void      enable(RenderContext& context) const override;
	   virtual void      disable(RenderContext& context) const override;

      virtual void      bindTexture(RenderContext& context, int stage, const Texture& uniform) override;
      virtual void      setConstantBuffer(RenderContext& context, const UniformBuffer& buffer) override;

   private:

    // loading
      bool loadVertexShader(DataStream& program);
      bool loadGeometryShader(DataStream& stream);
      bool loadPixelShader(DataStream& program);

      bool createInputLayout(const DataStream& stream);

    // members
      D3DDevice&              mDevice;
      ID3D11InputLayout*      mpInputLayout;
      ID3D11VertexShader*     mpVertexShader;
      ID3D11GeometryShader*   mpGeometryShader;
      ID3D11PixelShader*      mpPixelShader;
   };
}

#endif // D3D_CODE_PATH_H
