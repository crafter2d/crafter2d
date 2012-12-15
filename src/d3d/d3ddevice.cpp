
#include "d3ddevice.h"

D3DDevice::D3DDevice():
   Device(),
   mpDevice(NULL),
   mpContext(NULL),
   mpSwapChain(NULL),
   mpRenderTargetView(NULL)
{
}

bool D3DDevice::create(int window, int width, int height)
{
   D3D_FEATURE_LEVEL level;
   DXGI_SWAP_CHAIN_DESC sd;
   ZeroMemory(&sd, sizeof(sd));
   sd.BufferCount = 1;
   sd.BufferDesc.Width = width;
   sd.BufferDesc.Height = height;
   sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
   sd.BufferDesc.RefreshRate.Numerator = 60;
   sd.BufferDesc.RefreshRate.Denominator = 1;
   sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
   sd.OutputWindow = (HWND)window;
   sd.SampleDesc.Count = 1;
   sd.SampleDesc.Quality = 0;
   sd.Windowed = true;

   HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_NULL, NULL, 0, NULL, 0, D3D11_SDK_VERSION,
                                              &sd, &mpSwapChain, &mpDevice, &level, &mpContext);
   if ( FAILED(hr) )
   {
      return false;
   }

   ID3D11Texture2D* pbackbuffer = NULL;

   // create & set the rendertarget view
   hr = mpSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pbackbuffer);
   mpDevice->CreateRenderTargetView(pbackbuffer, NULL, &mpRenderTargetView);
   mpContext->OMSetRenderTargets(1, &mpRenderTargetView, NULL);

   // set the viewport
   CD3D11_VIEWPORT viewport(pbackbuffer, mpRenderTargetView);
   mpContext->RSSetViewports(1, &viewport);

   return true;
}

CodePath* D3DDevice::createCodePath(CodePath::PathType type)
{
   return NULL;
}
