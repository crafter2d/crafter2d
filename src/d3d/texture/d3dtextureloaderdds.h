
#ifndef D3D_TEXTURE_LOADER_DDS_H
#define D3D_TEXTURE_LOADER_DDS_H

#include <d3d11.h>

class DataStream;

namespace Graphics
{
   class D3DDevice;
   class D3DTexture;

   class D3DTextureLoaderDDS
   {
   public:

      D3DTexture* load(D3DDevice& device, DataStream& imagedata);
   };
}

#endif // D3D_TEXTURE_LOADER_DDS_H
