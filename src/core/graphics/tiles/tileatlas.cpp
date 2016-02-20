
#include "tileatlas.h"

c2d::TileAtlas::TileAtlas():
   mSheets()
{
}

int c2d::TileAtlas::lookup(const String & name) const
{
   for ( auto& sheet : mSheets )
   {
      int result = sheet.lookup(name);
      if ( result >= 0 )
      {
         return result;
      }
   }
   return -1;
}

c2d::TileSheet& c2d::TileAtlas::emplace()
{
   mSheets.emplace_back();
   return mSheets.back();
}

void c2d::TileAtlas::reserve(int count)
{
   mSheets.reserve(count);
}
