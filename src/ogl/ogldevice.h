
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
      explicit OGLDevice(ContentManager& contentmanager);
      virtual ~ OGLDevice();
      
    // overloads
      virtual bool create(int windowhandle, int width, int height) override;

    // creation
      virtual RenderContext*  createRenderContext() override;
      virtual CodePath*       createCodePath() override;
      virtual VertexBuffer*   createVertexBuffer() override;
      virtual IndexBuffer*    createIndexBuffer() override;
      virtual Texture*        createTexture(DataStream& data) override;
      virtual BlendState*     createBlendState(const BlendStateDesc& desc) override;
      virtual Font*           createFont(const String& name, int pointsize) override;

    // presenting
      virtual void present() override;

   private:
    // query
      bool supportCG() const;
      bool supportGLSL() const;

    // operations
      GLenum toGLBlendState(Graphics::BlendStateDesc::BlendFactor factor);

      FT_Library        mFreeTypeLib;
   };

};

#endif // D3D_DEVICE_H
