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
#include <assert.h>
#include "../defines.h"

INLINE
const NetStatistics::StatisticsItems& NetStatistics::items() const
{
   return MItems;
}

INLINE
NetStatistics::StatisticsItems& NetStatistics::items()
{
   return MItems;
}

INLINE
bool NetStatistics::hasCurrentItem() const
{
   return MPCurrentItem != NULL;
}

INLINE NetStatisticsItem& NetStatistics::currentItem()
{
   ASSERT_MSG(hasCurrentItem(), "Must have an current item.");
   return *MPCurrentItem;
}

INLINE void NetStatistics::currentItem(NetStatisticsItem* pcurrent)
{
   MPCurrentItem = pcurrent;
}

INLINE Uint32 NetStatistics::lastUpdate()
{
   return MLastUpdate;
}

INLINE void NetStatistics::lastUpdate(Uint32 tick)
{
   MLastUpdate = tick;
}
