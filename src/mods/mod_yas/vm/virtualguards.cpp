
#include "virtualguards.h"

#include "virtualguard.h"

VirtualGuards::VirtualGuards():
   mGuards()
{
}

const VirtualGuard& VirtualGuards::operator[](int index) const
{
   return *mGuards[index];
}

VirtualGuard& VirtualGuards::operator[](int index)
{
   return *mGuards[index];
}

// query

int VirtualGuards::size() const
{
   return mGuards.size();
}


// - Maintenance
      
void VirtualGuards::add(VirtualGuard* pguard)
{
   mGuards.push_back(pguard);
}

void VirtualGuards::updatePosition(int offset)
{
   for ( std::size_t index = 0; index < mGuards.size(); ++index )
   {
      VirtualGuard* pguard = mGuards[index];
      pguard->updatePosition(offset);
   }
}

// - Lookup

const VirtualGuard* VirtualGuards::findGuard(int pos) const
{
   const VirtualGuard* pbestguard = nullptr;
   int bestdist = 0xFFFFFFF;

   for ( std::size_t index = 0; index < mGuards.size(); ++index )
   {
      const VirtualGuard* pguard = mGuards[index];
      // range is [start,catch> as the last instruction before the catch is always a jump to final/end
      if ( pguard->locations[VirtualGuard::sStart] <= pos && pguard->locations[VirtualGuard::sCatch] >= pos )
      {
         int dist = pos - pguard->locations[VirtualGuard::sStart];
         if ( dist < bestdist )
         {
            bestdist = dist;
            pbestguard = pguard;
         }
      }
   }
   return pbestguard;
}
