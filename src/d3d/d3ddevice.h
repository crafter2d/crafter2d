
#ifndef D3D_DEVICE_H
#define D3D_DEVICE_H

#include "core/graphics/device.h"

#include <d3d11.h>

class D3DDevice : public Device
{
public:
   D3DDevice();

 // overloads
   virtual bool create(int window, int width, int height);

   virtual CodePath* createCodePath(CodePath::PathType type);

private:
   ID3D11Device*           mpDevice;
   ID3D11DeviceContext*    mpContext;
   IDXGISwapChain*         mpSwapChain;
   ID3D11RenderTargetView* mpRenderTargetView;
};

#endif // D3D_DEVICE_H
