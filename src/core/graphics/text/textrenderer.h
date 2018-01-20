
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <memory>
#include <unordered_map>
#include <vector>

#include "core/math/matrix4.h"

#include "glyphatlas.h"

class String;
class Vector;

namespace Graphics
{
   class Device;
   class Effect;
   class Font;
   class GlyphAtlas;
   class VertexBuffer;
   class IndexBuffer;
   class UniformBuffer;
   class RenderContext;
   class TextLayout;
   class Viewport;

   class TextRenderer final
   {
   public:
      TextRenderer();
      ~TextRenderer();

    // get/set
      GlyphAtlas& getGlyphAtlas() {
         return *mAtlas;
      }

    // operations
      bool initialize(Device& device);

      void draw(const TextLayout& layout);
      void render(RenderContext& context);

    // font
      Font& getFont(const String& name);

    // notifications
      void viewportChanged(RenderContext& context, const Viewport& viewport);

   private:
      using Fonts = std::unordered_map<String, Font*>;
      using Texts = std::vector<const TextLayout*>;

      friend class TextLayout;

      struct ConstantBuffer
      {
         Matrix4 projection;
         Matrix4 world;
         Matrix4 object;
      };

      struct TransformBuffer
      {
         Matrix4 transforms[92];
      };

    // font
      bool hasFont(const String& name);
      void addFont(Font* pfont);

    // data
      Fonts             mFonts;
      Texts             mTexts;
      ConstantBuffer    mConstants;
      Device*           mpDevice;
      std::unique_ptr<GlyphAtlas> mAtlas;
      Effect*           mpEffect;
      VertexBuffer*     mpVB;
      IndexBuffer*      mpIB;
      UniformBuffer*    mpUB;
      UniformBuffer*    mpTransformUB;      
   };
}

#endif // TEXT_RENDERER_H
