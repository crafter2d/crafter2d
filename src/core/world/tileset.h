/***************************************************************************
 *   Copyright (C) 2006 by Jeroen Broekhuizen                              *
 *   jengine.sse@live.nl                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef TILESET_H_
#define TILESET_H_

#include <vector>

#include "core/core_base.h"
#include "core/content/content.h"
#include "core/graphics/texturecoordinate.h"

namespace Graphics
{
   class Device;
   class Texture;
}

enum TileInfoFlag {
   TileAnimate    = 1,
   TileSolid      = 2,
   TileRotated    = 4,
};

/// Structure contains animation information for a tile. Every tile can have a certain amount of information.
struct CORE_API TileInfo {
   c2d::TextureCoordinate coords;
   Vector offset;
   int    sheet;
   int    flag;

   TileInfo();
};

struct CORE_API TileAnimation {
   int   anim_index;
   int   anim_length;
   float anim_speed;
   float anim_update;

   TileAnimation();
};

/// Implements a tileset container. In this container we can store information about every tile in the tile bitmap.
class CORE_API TileSet : public IContent
{
public:
   using TileInfos = std::vector<TileInfo>;

   TileSet(int tilewidth, int tileheight);
   virtual ~TileSet();

   TileInfo& operator[](int index) {
      return mInfos[index];
   }

 // get/set
   int size() const {
      return mInfos.size();
   }

   int getTileWidth() const {
      return mTileWidth;
   }

   int getTileHeight() const {
      return mTileHeight;
   }

   void setTileInfos(TileInfos&& infos);
   
 // operations
   bool update(float tick);

private:
   
 // data
   TileInfos   mInfos;
   int         mTileWidth;
   int         mTileHeight;
};

#endif // TILESET_H_
