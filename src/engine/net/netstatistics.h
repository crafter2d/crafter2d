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
#ifndef NETSTATISTICS_H_
#define NETSTATISTICS_H_

#include <vector>

#include "core/defines.h"

class NetStatisticsItem;

/// NetStatistics
/// Maintains statistics about the network transmissions during the run
/// of a connection. This information contains counters for packages send
/// and received and the corresponding bytes per second.
class NetStatistics
{
public:
   // Types
      typedef std::vector<NetStatisticsItem> StatisticsItems;

   // Construction
      NetStatistics();
      ~NetStatistics();

   // Get/set interface
      const StatisticsItems&  items() const;

      bool                    hasCurrentItem() const;
      NetStatisticsItem&      currentItem();
      void                    currentItem(NetStatisticsItem* pcurrent);

      uint32_t                lastUpdate() const;
      void                    lastUpdate(uint32_t tick);

   // operations
      void send(uint32_t size);
      void received(uint32_t size);
      void update(uint32_t tick);

private:
   // Get/set interface
      StatisticsItems& items();

   // Operations
      void appendNewItem();
      void ensureValidSize();

   // Members
      StatisticsItems      MItems;
      NetStatisticsItem*   MPCurrentItem;
      uint32_t             MLastUpdate;
      uint32_t             MLimit;
};

#ifdef JENGINE_INLINE
#  include "netstatistics.inl"
#endif

#endif // NETSTATISTICS_H_
