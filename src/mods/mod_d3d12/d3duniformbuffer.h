
#ifndef D3D_UNIFORM_BUFFER_H
#define D3D_UNIFORM_BUFFER_H

#include "mod_d3d12.h"

#include "core/graphics/uniformbuffer.h"

struct ID3D11Buffer;

namespace Graphics
{
   class MOD_API D3DUniformBuffer : public UniformBuffer
   {
   public:
      D3DUniformBuffer();
      virtual ~D3DUniformBuffer();

    // get/set
      ID3D11Buffer* getBuffer() const {
         return mpBuffer;
      }

    // operations
      virtual bool create(Device& device, UNIFORM_BUFFER_DESC* pdescs, int nr) override;
      virtual void release() override;

      virtual void set(RenderContext& context, const void* pdata, const int size) override;
               
   private:

    // helpers
      int determineSize(UNIFORM_BUFFER_DESC* pdescs, int nr);

    // data
      ID3D11Buffer* mpBuffer;
   };
}

#endif // D3D_UNIFORM_BUFFER_H
