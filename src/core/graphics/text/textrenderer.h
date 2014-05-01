
#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>
class String;

namespace Graphics
{
   class Device;
   class Font;
   class GlyphProvider;

   class TextRenderer
   {
   public:
      TextRenderer();

      bool initialize(Device& device);
      void draw(Font& font, float fontsizeem, const String& text);

    // font
      
      Font& getFont(const String& name);
      
     

   private:
      typedef std::map<String, Font*> Fonts;

    // font
      bool hasFont(const String& name);
      void addFont(Font* pfont);

    // data
      Device*        mpDevice;
      GlyphProvider* mpProvider;
      Fonts          mFonts;
   };
}

#endif // TEXT_RENDERER_H
