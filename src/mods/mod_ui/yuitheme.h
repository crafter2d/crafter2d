#ifndef YUI_THEME_H
#define YUI_THEME_H

#include "core/content/content.h"

namespace Graphics
{
   class RenderContext;
}

class String;

namespace c2d
{
   class ContentManager;
   class TileAtlas;

   class YuiTheme : public IContent
   {
   public:
      explicit YuiTheme(TileAtlas* patlas);

      const TileAtlas& getAtlas() const {
         return *mpAtlas;
      }

    // operations
      void bind(Graphics::RenderContext& context);

   private:

    // members
      TileAtlas* mpAtlas;
   };
}

#endif // YUI_THEME_H
