
#ifndef D3D_TEXTURE_LOADER_H
#define D3D_TEXTURE_LOADER_H

class String;

namespace Graphics
{
   class D3D11Device;
   class D3DTexture;

   class D3DTextureLoader
   {
   public:

      D3DTexture* loadTexture(D3D11Device& device, const String& filename);

   private:

   };
}

#endif // D3D_TEXTURE_LOADER_H
