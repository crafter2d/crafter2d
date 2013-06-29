
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

      virtual bool load(VertexInputLayout& layout, const String& vertex, const String& fragment) override;

   private:

      bool readShaderFile(const String& filename, DataStream& buffer);

      bool loadVertexProgram(VertexInputLayout& layout, const String& filename);
      bool loadPixelProgram(const String& filename);

      bool createInputLayout(const VertexInputLayout& layout, DataStream& stream);

    // members
      D3DDevice& mDevice;
      ID3D11InputLayout*  mpInputLayout;
      ID3D11VertexShader* mpVertexShader;
      ID3D11PixelShader*  mpPixelShader;
   };
}

#endif // D3D_CODE_PATH_H
