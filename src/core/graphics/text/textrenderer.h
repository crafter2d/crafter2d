
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>

#include "core/math/matrix4.h"

class String;
class Vector;

namespace Graphics
{
   class Device;
   class Effect;
   class Font;
   class GlyphProvider;
   class VertexBuffer;
   class IndexBuffer;
   class UniformBuffer;
   class RenderContext;
   class TextLayout;
   class Viewport;

   class TextRenderer
   {
   public:
      TextRenderer();

    // operations
      bool initialize(Device& device);

      void draw(RenderContext& context, TextLayout& layout);
      void draw(RenderContext& context, const Vector& position, Font& font, float fontsizeem, const String& text);

    // font
      Font& getFont(const String& name);

    // notifications
      void viewportChanged(RenderContext& context, const Viewport& viewport);

   private:
      typedef std::map<String, Font*> Fonts;

      struct ConstantBuffer
      {
         Matrix4 projection;
         Matrix4 world;
         Matrix4 object;
      };

    // font
      bool hasFont(const String& name);
      void addFont(Font* pfont);

    // data
      Device*           mpDevice;
      Effect*           mpEffect;
      VertexBuffer*     mpVB;
      IndexBuffer*      mpIB;
      UniformBuffer*    mpUB;
      ConstantBuffer    mConstants;
      GlyphProvider*    mpProvider;
      Fonts             mFonts;
   };
}

#endif // TEXT_RENDERER_H
