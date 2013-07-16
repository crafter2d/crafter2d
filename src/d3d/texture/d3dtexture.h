
#ifndef D3D_TEXTURE_H
#define D3D_TEXTURE_H

#include "core/graphics/texture.h"

struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;

namespace Graphics
{
   class D3DTexture : public Texture
   {
   public:
      explicit D3DTexture(ID3D11ShaderResourceView* presource);

      virtual bool   create(Device& device, const TextureInfo& info) override;

      virtual void   enable(RenderContext& context) const override;
      virtual void   disable(RenderContext& context) const override;

      virtual bool   isValid() const override;

   private:

      ID3D11ShaderResourceView* mpResource;
      ID3D11SamplerState*       mpSampler;
   };
}

#endif // D3D_TEXTURE_H
