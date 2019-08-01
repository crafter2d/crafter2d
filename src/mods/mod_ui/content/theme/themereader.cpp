
#include "themereader.h"

#include "core/content/contentmanager.h"
#include "core/graphics/tiles/tileatlas.h"
#include "core/streams/datastream.h"

#include "../../yuitheme.h"

namespace c2d
{
   IContent* ThemeReader::read(DataStream& stream)
   {
      String atlas;
      stream.readString(atlas);
      auto patlas = getContentManager().loadContent<TileAtlas>(atlas);
      return new YuiTheme(patlas);
   }
}