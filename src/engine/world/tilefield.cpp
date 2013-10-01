
#include "tilefield.h"

#include "core/streams/datastream.h"
#include "core/defines.h"

TileField::TileField():
   mpMap(NULL)
{
}

TileField::~TileField()
{
   delete[] mpMap;
   mpMap = NULL;
}

// get/set

int TileField::tileindex(LayerLevel level, int x, int y) const
{
   return (y * mWidth + x) * 3 + (uint8_t)level;
}

uint8_t TileField::get(LayerLevel level, int x, int y) const
{
   ASSERT(x >= 0 && y >= 0 && x < mWidth && y < mHeight);
   int index = tileindex(level, x, y);
   return mpMap[index];
}

void TileField::set(LayerLevel level, int x, int y, uint8_t tile)
{
   if ( x >= 0 && y >= 0 && x < mWidth && y < mHeight )
   {
      int index = tileindex(level, x, y);
      mpMap[index] = tile;
   }
}

// - Operations

void TileField::create(int width, int height)
{
   mpMap = new uint8_t[width * height * 3];
   memset(mpMap, 255, sizeof(uint8_t) * width * height * 3);

   mWidth = width;
   mHeight = height;
}

void TileField::resize(int width, int height)
{
   uint8_t* pmap = new uint8_t[width * height * 3];
   memset(pmap, -1, sizeof(uint8_t) * width * height * 3);

   int minwidth = MIN(width, mWidth);
   int minheight = MIN(height, mHeight);
   int nextrow = sizeof(uint8_t) * width * 3;

   uint8_t* ptr = pmap;
   for ( int y = 0; y < minheight; ++y )
   {
      memcpy(ptr, mpMap, sizeof(uint8_t) * minwidth * 3);
      ptr += nextrow;
   }

   delete[] mpMap;
   mpMap = pmap;
   mWidth = width;
   mHeight = height;
}

// save & load
   
void TileField::load(DataStream& stream)
{
   stream >> mWidth >> mHeight;

   int size = sizeof(uint8_t) * mWidth * mHeight * 3;
   mpMap = new uint8_t[size];

   stream.readBlob(mpMap, size);
}

void TileField::write(DataStream& stream) const
{
   stream << mWidth << mHeight;
   
   int size = sizeof(uint8_t) * mWidth * mHeight * 3;
   stream.writeBlob(mpMap, size);
}
