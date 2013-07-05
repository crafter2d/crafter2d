
#ifndef OGL_DEVICE_H
#define OGL_DEVICE_H

#include "core/graphics/device.h"

#include <GL/GLee.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Graphics
{
   class OGLDevice : public Device
   {
   public:
      OGLDevice();
      virtual ~ OGLDevice();
      
    // overloads
      virtual bool create(int windowhandle, int width, int height);

      virtual RenderContext*  createRenderContext() override;
      virtual CodePath*       createCodePath() override;
      virtual VertexBuffer*   createVertexBuffer(VertexInputLayout& layout) override;
      virtual IndexBuffer*    createIndexBuffer() override;
      virtual Texture*        createTexture(const String& filename) override;
      virtual BlendState*     createBlendState(const BlendStateDesc& desc) override;
      virtual Font*           createFont(const String& name, int pointsize) override;

   private:
    // query
      bool supportCG() const;
      bool supportGLSL() const;

    // operations
      GLenum toGLBlendState(Graphics::BlendStateDesc::BlendFactor factor);

      FT_Library  mFreeTypeLib;
   };

};

#endif // D3D_DEVICE_H
