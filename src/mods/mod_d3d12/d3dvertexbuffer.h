
#ifndef D3D_VERTEX_BUFFER_H
#define D3D_VERTEX_BUFFER_H

#include "core/graphics/vertexbuffer.h"

struct ID3D11Buffer;

namespace Graphics
{
   class D3D11Device;

   class D3DVertexBuffer : public VertexBuffer
   {
   public:
      explicit D3DVertexBuffer(D3D11Device& device);
      virtual ~D3DVertexBuffer();

      virtual bool create(const VertexLayout& layout, int length, int usage) override;
      virtual void release() override;
      
      virtual void update(RenderContext& context, void* pdata, int count) override;
      
      virtual void*  lock(RenderContext& context) override;
      virtual void   unlock(RenderContext& context) override;

      virtual void   enable(RenderContext& context) const override;
      virtual void   disable(RenderContext& context) const override;

   private:

    // data
      D3D11Device&   mDevice;
      ID3D11Buffer*  mpBuffer;
      int            mStride;
   };
}

#endif // D3D_VERTEX_BUFFER_H
