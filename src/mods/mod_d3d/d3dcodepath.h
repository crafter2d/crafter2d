
#ifndef D3D_CODE_PATH_H
#define D3D_CODE_PATH_H

#include "core/graphics/codepath.h"

class DataStream;

struct ID3D11InputLayout;
struct ID3D11VertexShader;
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

      virtual bool      create(VertexLayout* playout, DataStream& vertexshader, DataStream& pixelshader) override;
      virtual void      release() override;

      virtual void      enable(RenderContext& context) const override;
	   virtual void      disable(RenderContext& context) const override;

      virtual bool      bindTexture(RenderContext& context, int stage, const Texture& uniform) override;

   private:

      bool loadVertexProgram(DataStream& program);
      bool loadPixelProgram(DataStream& program);

      bool createInputLayout(const DataStream& stream);

    // members
      D3DDevice& mDevice;
      ID3D11InputLayout*  mpInputLayout;
      ID3D11VertexShader* mpVertexShader;
      ID3D11PixelShader*  mpPixelShader;
   };
}

#endif // D3D_CODE_PATH_H
