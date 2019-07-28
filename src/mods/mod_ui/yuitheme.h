#ifndef YUI_THEME_H
#define YUI_THEME_H

#include "core/content/content.h"

class String;

namespace c2d
{
   class ContentManager;

   class YuiTheme : public IContent
   {
   public:

    // operations
      bool load(ContentManager& contentmgr, const String& filename);
   };
}

#endif // YUI_THEME_H
