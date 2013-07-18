
#ifndef D3D_UNIFORM_BUFFER_H
#define D3D_UNIFORM_BUFFER_H

#include "core/graphics/uniformbuffer.h"

struct ID3D11Buffer;

namespace Graphics
{
   class D3DUniformBuffer : public UniformBuffer
   {
   public:
      D3DUniformBuffer();
      virtual ~D3DUniformBuffer();

      virtual bool create(Device& device, UNIFORM_BUFFER_DESC* pdescs, int nr) override;
      virtual void release() override;

      virtual void set(RenderContext& context, const void* pdata) override;

      virtual void enable(RenderContext& context) const override;
         
   private:

    // helpers
      int determineSize(UNIFORM_BUFFER_DESC* pdescs, int nr);

    // data
      ID3D11Buffer* mpBuffer;
   };
}

#endif // D3D_UNIFORM_BUFFER_H
