/***************************************************************************
 *   Copyright (C) 2004 by Jeroen Broekhuizen                              *
 *   jeroen@nwnx.homedns.org                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "netstatisticsitem.h"

#include "netstatistics.h"
#ifndef JENGINE_INLINE
#  include "netstatistics.inl"
#endif

NetStatistics::NetStatistics():
   MItems(),
   MPCurrentItem(NULL),
   MLastUpdate(0),
   MLimit(JENGINE_STATISTICS_LIMIT)
{
   appendNewItem();
}

NetStatistics::~NetStatistics()
{
   currentItem(NULL);
   items().clear();
}

//////////////////////////////////////////////////////////////////////////
// - Operations
//////////////////////////////////////////////////////////////////////////

void NetStatistics::addPackageSend(Uint32 size)
{
   currentItem().addPackageSend();
   currentItem().addBytesSend(size);
}

void NetStatistics::addPackageReceived(Uint32 size)
{
   currentItem().addPackageReceived();
   currentItem().addBytesReceived(size);
}

void NetStatistics::update(Uint32 tick)
{
   if ( tick - lastUpdate() >= 1000 )
   {
      appendNewItem();
      lastUpdate(tick);
   }
}

void NetStatistics::appendNewItem()
{
   items().push_back(NetStatisticsItem());
   currentItem(&items()[items().size()-1]);
   ASSERT(hasCurrentItem() != 0);

   ensureValidSize();
}

void NetStatistics::ensureValidSize()
{
   if ( MItems.size() > MLimit )
   {
      MItems.erase(MItems.begin());
   }
}
