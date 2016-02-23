
#ifndef DEVICE_H
#define DEVICE_H

#include "core/core_base.h"

#include "core/graphics/codepath.h"
#include "core/graphics/blendstatedesc.h"

namespace c2d
{
   class ContentManager;
}

class DataStream;
class GameWindow;
class String;

namespace Graphics
{
   class Effect;
   class Font;
   class BlendState;
   class GlyphProvider;
   class RenderContext;
   class RenderTarget;
   class Texture;
   class IndexBuffer;
   class VertexBuffer;

   struct TextureDescription;

   class CORE_API Device
   {
   public:
      Device();
      virtual ~Device();
      
    // get/set
      c2d::ContentManager& getContentManager();
      void                 setContentManager(c2d::ContentManager& contentmanager);

      RenderContext&  getContext();
      Font&           getFont(const String& name);

    // operations
      virtual bool create(GameWindow& window);
      virtual void destroy();
      virtual void present() = 0;

    // creation
      virtual CodePath*       createCodePath() = 0;
      virtual VertexBuffer*   createVertexBuffer() = 0;
      virtual IndexBuffer*    createIndexBuffer() = 0;
      virtual Texture*        createTexture(int width, int height, int bytesperpixel) = 0;
      virtual Texture*        createTexture(const TextureDescription& desc) = 0;
      virtual RenderTarget*   createRenderTarget() = 0;
      virtual BlendState*     createBlendState(const BlendStateDesc& desc) = 0;
      virtual GlyphProvider*  createGlyphProvider(Font& font) = 0;
              Effect*         createEffect(const String& filename);

   protected:
    // creations
      virtual Font*           createFont(const String& name) = 0;
      virtual RenderContext*  createRenderContext() = 0;

   private:
      friend class TextRenderer;

    // data
      c2d::ContentManager* mpContentManager;
      RenderContext*       mpContext;
   };
};

#endif // DEVICE_H
