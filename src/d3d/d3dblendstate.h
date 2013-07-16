
#ifndef D3D_BLEND_STATE_H
#define D3D_BLEND_STATE_H

#include "core/graphics/blendstate.h"
#include "core/graphics/blendstatedesc.h"

struct ID3D11BlendState;
struct ID3D11Device;
struct ID3D11DeviceContext;
enum D3D11_BLEND;

namespace Graphics
{
   class BlendStateDesc;

   class D3DBlendState : public BlendState
   {
   public:

      bool create(ID3D11Device& device, const BlendStateDesc& desc);

      void enable(ID3D11DeviceContext& context) const;

   private:

    // helpers
      D3D11_BLEND toD3DBlend(BlendStateDesc::BlendFactor factor);

    // data
      ID3D11BlendState* mpBlendState;
   };
};

#endif // D3D_BLEND_STATE_H
