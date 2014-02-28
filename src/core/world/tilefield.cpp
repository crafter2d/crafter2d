
#include "tilefield.h"

#include "core/streams/datastream.h"
#include "core/defines.h"

TileField::TileField():
   mpMap(NULL),
   mWidth(0),
   mHeight(0)
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

void TileField::create(int width, int height, uint8_t* ptiledata)
{
   mWidth = width;
   mHeight = height;
   mpMap = ptiledata;
}

void TileField::resize(int width, int height)
{
   uint8_t* pmap = new uint8_t[width * height * 3];
   memset(pmap, -1, sizeof(uint8_t) * width * height * 3);

   int minwidth = MIN(width, mWidth);
   int minheight = MIN(height, mHeight);

   int srcsize = sizeof(uint8_t) * width * 3;
   int destsize = sizeof(uint8_t) * mWidth * 3;

   uint8_t* psrc = pmap;
   uint8_t* pdest = mpMap;

   for ( int y = 0; y < minheight; ++y )
   {
      memcpy(psrc, pdest, sizeof(uint8_t) * minwidth * 3);
      psrc += srcsize;
      pdest += destsize;
   }

   delete[] mpMap;
   mpMap = pmap;
   mWidth = width;
   mHeight = height;
}

// - Loading

// save & load
   
void TileField::load(DataStream& stream)
{
   stream >> mWidth >> mHeight;

   int size = sizeof(uint8_t) * mWidth * mHeight * 3;
   mpMap = new uint8_t[size];

   stream.readBlob(mpMap, size);
}
