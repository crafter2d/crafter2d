
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
