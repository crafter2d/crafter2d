
#include "virtualguards.h"

#include "virtualguard.h"

VirtualGuards::VirtualGuards():
   mGuards()
{
}

// - Maintenance
      
void VirtualGuards::updatePosition(int offset)
{
   for ( auto& guard : mGuards )
   {
      guard.updatePosition(offset);
   }
}

// - Lookup

const VirtualGuard* VirtualGuards::findGuard(int pos) const
{
   const VirtualGuard* pbestguard = nullptr;
   int bestdist = 0xFFFFFFF;

   for ( auto& guard : mGuards )
   {
      // range is [start,catch> as the last instruction before the catch is always a jump to final/end
      if ( guard.locations[VirtualGuard::sStart] <= pos && guard.locations[VirtualGuard::sCatch] >= pos )
      {
         int dist = pos - guard.locations[VirtualGuard::sStart];
         if ( dist < bestdist )
         {
            bestdist = dist;
            pbestguard = &guard;
         }
      }
   }
   return pbestguard;
}
