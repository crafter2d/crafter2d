
#ifndef OGL_DEVICE_H
#define OGL_DEVICE_H

#include "core/graphics/device.h"

#include <GL/GLee.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "cgengine.h"

namespace Graphics
{

   class OGLDevice : public Device
   {
   public:
      OGLDevice();
      virtual ~ OGLDevice();

    // get/set
      CGEngine& getCG() { return mCG; }

    // overloads
      virtual bool create(int windowhandle);

      virtual RenderContext*  createRenderContext();
      virtual CodePath*       createCodePath(CodePath::PathType type);
      virtual VertexBuffer*   createVertexBuffer();
      virtual IndexBuffer*    createIndexBuffer();
      virtual Texture*        createTexture(const String& filename);
      virtual BlendState*     createBlendState(const BlendStateDesc& desc);
      virtual Font*           createFont(const String& name, int pointsize);

   private:
    // query
      bool supportCG() const;
      bool supportGLSL() const;

    // operations
      GLenum toGLBlendState(Graphics::BlendStateDesc::BlendFactor factor);

      CGEngine    mCG;
      FT_Library  mFreeTypeLib;
   };

};

#endif // D3D_DEVICE_H
