
#ifndef DEVICE_H
#define DEVICE_H

#include "core/core_base.h"

#include "core/graphics/codepath.h"
#include "core/graphics/blendstatedesc.h"

class String;

namespace Graphics
{
   class Font;
   class BlendState;
   class RenderContext;
   class Texture;
   class IndexBuffer;
   class VertexBuffer;
   class VertexInputLayout;

   class CORE_API Device
   {
   public:
      virtual ~Device();

      virtual bool create(int windowhandle, int width, int height) = 0;

      virtual RenderContext*  createRenderContext() = 0;
      virtual CodePath*       createCodePath(CodePath::PathType type) = 0;
      virtual VertexBuffer*   createVertexBuffer(VertexInputLayout& layout) = 0;
      virtual IndexBuffer*    createIndexBuffer() = 0;
      virtual Texture*        createTexture(const String& filename) = 0;
      virtual BlendState*     createBlendState(const BlendStateDesc& desc) = 0;
      virtual Font*           createFont(const String& name, int pointsize) = 0;
   };
};

#endif // DEVICE_H
