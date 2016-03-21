
#ifndef OGL_DEVICE_H
#define OGL_DEVICE_H

#include "core/graphics/device.h"

#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Graphics
{
   class OGLDevice : public Device
   {
   public:
      OGLDevice();
      virtual ~OGLDevice();

    // overloads
      virtual bool create(GameWindow& window) override;
      virtual void resize(int width, int height) override;

    // creation
      virtual RenderContext*  createRenderContext() override;
      virtual CodePath*       createCodePath() override;
      virtual VertexBuffer*   createVertexBuffer() override;
      virtual IndexBuffer*    createIndexBuffer() override;
      virtual Texture*        createTexture(int width, int height, int bytesperpixel) override;
      virtual Texture*        createTexture(const TextureDescription& desc) override;
      virtual RenderTarget*   createRenderTarget() override;
      virtual BlendState*     createBlendState(const BlendStateDesc& desc) override;
      virtual GlyphProvider*  createGlyphProvider(Font& font);

    // presenting
      virtual void present() override;

   protected:
    // overrides
      virtual Font*           createFont(const String& name) override;

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
