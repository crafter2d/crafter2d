
#ifndef TILE_FIELD_H
#define TILE_FIELD_H

#include <stdint.h>
#include <vector>

#include "core/core_base.h"

#include "layertype.h"

class DataStream;
class TileRow;

class CORE_API TileField
{
public:
   enum MapLayer { eFront, eMid, eBack };

   TileField();
   ~TileField();

 // get/set
   uint8_t  get(LayerLevel level, int x, int y) const;
   void     set(LayerLevel level, int x, int y, uint8_t tile);

 // operations
   void create(int width, int height, uint8_t* ptiledata);
   void resize(int width, int height);

 // loading (for conversion only!)
   void load(DataStream& stream);
   
private:
 // operations
   int tileindex(LayerLevel level, int x, int y) const;
   
 // data
   uint8_t * mpMap;
   int       mWidth;
   int       mHeight;
};

#endif // TILE_FIELD_H
