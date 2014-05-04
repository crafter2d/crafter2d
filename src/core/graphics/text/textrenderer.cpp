
#include "textrenderer.h"

#include "core/defines.h"
#include "core/system/exception.h"
#include "core/graphics/device.h"
#include "core/graphics/font.h"
#include "glyphprovider.h"

namespace Graphics
{
   TextRenderer::TextRenderer():
      mpProvider(NULL)
   {
   }

   bool TextRenderer::initialize(Device& device)
   {
      mpDevice = &device;

      return true;
   }

   void TextRenderer::draw(Font& font, float fontsize, const String& text)
   {
      for ( int index = 0; index < text.length(); ++index )
      {
         UChar ch = text[0];
         Glyph* pglyph = mpProvider->getGlyph(ch, fontsize);
         if ( pglyph != NULL )
         {

         }
      }
   }

   // - Font
   bool TextRenderer::hasFont(const String& name)
   {
      return mFonts.find(name) != mFonts.end();
   }

   Font& TextRenderer::getFont(const String& name)
   {
      Font* pfont = NULL;
      Fonts::iterator it = mFonts.find(name);
      if ( it == mFonts.end() )
      {
         pfont = mpDevice->createFont(name);
         mpProvider = mpDevice->createGlyphProvider(*pfont);
         if ( mpProvider == NULL )
         {
            throw new c2d::Exception(UTEXT("Could not create glyph provider!"));
         }
      }
      else
      {
         pfont = it->second;
      }

      return *pfont;
   }

   void TextRenderer::addFont(Font* pfont)
   {
      mFonts.insert(std::make_pair(pfont->getFamilyName(), pfont));
   }
}
