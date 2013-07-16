
#include "d3dtextureloaderdds.h" 

#include <memory>

#include "core/graphics/textureinfo.h"
#include "core/smartptr/autoptr.h"
#include "core/vfs/filesystem.h"
#include "core/vfs/file.h"

#include "d3d/d3ddevice.h"

#include "d3dtexture.h"
#include "dds.h"


using namespace DirectX;

static HRESULT loadTextureDataFromFile(const String& filename, std::unique_ptr<uint8_t[]>& ddsData, DDS_HEADER** header, uint8_t** bitData, uint* bitSize)
{
   AutoPtr<File> file = FileSystem::getInstance().open(filename, File::ERead | File::EBinary);
   if ( !file.hasPointer() )
      return E_FAIL;

   int size = file->size();
   ddsData.reset(new (std::nothrow) uint8_t[size]);
   if ( !ddsData )
      return E_FAIL;

   int read = file->read(ddsData.get(), size);
   if ( read < size )
      return E_FAIL;

   uint magicNumber = *(uint*)(ddsData.get());
   if ( magicNumber != DDS_MAGIC )
      return E_FAIL;

   auto hdr = reinterpret_cast<DDS_HEADER*>(ddsData.get() + sizeof(uint));

   // Verify header to validate DDS file
   if ( hdr->size != sizeof(DDS_HEADER) ||
       hdr->ddspf.size != sizeof(DDS_PIXELFORMAT) )
   {
      return E_FAIL;
   }

   // Check for DX10 extension
   bool bDXT10Header = false;
   if ( (hdr->ddspf.flags & DDS_FOURCC) && (MAKEFOURCC( 'D', 'X', '1', '0' ) == hdr->ddspf.fourCC) )
   {
      // Must be long enough for both headers and magic value
      if (size < ( sizeof(DDS_HEADER) + sizeof(uint32_t) + sizeof(DDS_HEADER_DXT10) ) )
      {
         return E_FAIL;
      }

      bDXT10Header = true;
   }

   // setup the pointers in the process request
    *header = hdr;
    ptrdiff_t offset = sizeof( uint32_t ) + sizeof( DDS_HEADER )
                       + (bDXT10Header ? sizeof( DDS_HEADER_DXT10 ) : 0);
    *bitData = ddsData.get() + offset;
    *bitSize = size - offset;

    return S_OK;
}

//------------------------------------------------------------------------------------------
// - D3DTextureLoaderDDS class

namespace Graphics
{

D3DTexture* D3DTextureLoaderDDS::load(D3DDevice& device, const String& filename)
{
   DDS_HEADER* pheader = NULL;
   uint8_t* bitData = NULL;
   uint  bitSize = 0;

   std::unique_ptr<uint8_t[]> ddsData;
   HRESULT hr = loadTextureDataFromFile(filename, ddsData, &pheader, &bitData, &bitSize);
   if ( FAILED(hr) )
   {
      return false;
   }

   ID3D11Resource* texture;
   ID3D11ShaderResourceView* textureView;

   hr = CreateTextureFromDDS(&device.getDevice(), pheader, bitData, bitSize, 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, false, &texture, &textureView);
   if ( FAILED(hr) )
   {
      return false;
   }
   
   texture->Release();

   D3DTexture* presult = new D3DTexture(textureView);

   TextureInfo info;
   info.setWidth(pheader->width);
   info.setHeight(pheader->height);

   presult->create(device, info);

   return presult;
}

} // namespace Graphics
