
#include "tilesheet.h"

#include "core/graphics/texture.h"

c2d::TileSheet::TileSheet():
   mpTexture(nullptr),
   mLookup(),
   mTiles()
{
}

void c2d::TileSheet::add(const String & name, const Tile & tile)
{
   mLookup.insert({ name, mTiles.size() });
   mTiles.push_back(tile);
}

int c2d::TileSheet::lookup(const String & name) const
{
   auto it = mLookup.find(name);
   return it != mLookup.end() ? it->second : -1;
}

void c2d::TileSheet::bind(Graphics::RenderContext& context) const
{
   mpTexture->enable(context, 0);
}