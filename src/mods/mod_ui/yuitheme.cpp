
#include "yuitheme.h"

#include "core/content/contentmanager.h"
#include "core/graphics/tiles/tileatlas.h"
#include "core/string/string.h"



namespace c2d
{
   bool YuiTheme::load(ContentManager& contentmgr, const String& filename)
   {
      bool result = false;
      
      contentmgr.loadContent<YuiTheme>(filename);

      return result;
   }
}
