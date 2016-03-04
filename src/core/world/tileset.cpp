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
#include "tileset.h"
#ifndef JENGINE_INLINE
#  include "tileset.inl"
#endif

#include <cstring>

#include "core/content/contentmanager.h"
#include "core/graphics/device.h"
#include "core/graphics/texture.h"
#include "core/log/log.h"
#include "core/string/string.h"

//////////////////////////////////////////////////////////////////////////
// - TileInfo
//////////////////////////////////////////////////////////////////////////

TileInfo::TileInfo():
   coords(),
   offset(),
   sheet(0),
   flag(0)
{
}

TileAnimation::TileAnimation():
   anim_index(0),
   anim_length(0),
   anim_speed(0),
   anim_update(0)
{
}

//////////////////////////////////////////////////////////////////////////
// - Construction
//////////////////////////////////////////////////////////////////////////

TileSet::TileSet():
   IContent(),
   mpInfo(NULL),
   mTileCount(0),
   mTileWidth(0),
   mTileHeight(0)
{
}

TileSet::~TileSet()
{
   delete[] mpInfo;
   mTileCount = 0;
}

// - Get/set

void TileSet::setTileCount(int count)
{
   if ( count != mTileCount )
   {
      mTileCount = count;

      delete mpInfo;
      mpInfo = new TileInfo[count];
      memset(mpInfo, 0, sizeof(TileInfo) * count);
   }
}

// - Operations

bool TileSet::update(float tick)
{
   bool dirty = false;

   for ( int index = 0; index < mTileCount; ++index )
   {
      TileInfo& info = mpInfo[index];

      /*
      if ( info.flag & TileAnimate )
      {
         // only process animated tiles
         if ( tick - info.anim_update > info.anim_speed )
         {
            if (++info.anim_index == info.anim_length)
               info.anim_index = 0;

            info.anim_update = tick;
            dirty = true;
         }

         index += info.anim_length - 1;
      }
      */
   }

   return dirty;
}
