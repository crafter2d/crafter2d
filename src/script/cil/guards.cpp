
#include "guards.h"

#include "guard.h"

namespace CIL
{
   Guards::Guards():
      mGuards()
   {
   }

   Guards::~Guards()
   {
      clear();
   }

   const Guards& Guards::operator=(const Guards& that)
   {
      for ( std::size_t index = 0; index < that.mGuards.size(); ++index )
      {
         mGuards.push_back(that.mGuards[index]);
      }
      return *this;
   }

   const Guard& Guards::operator[](int index) const
   {
      return *mGuards[index];
   }

   // - Query

   int Guards::size() const
   {
      return (int)mGuards.size();
   }

   // - Operations

   void Guards::add(Guard* pguard)
   {
      mGuards.push_back(pguard);
   }

   void Guards::clear()
   {
      for ( std::size_t index = 0; index < mGuards.size(); ++index )
      {
         delete mGuards[index];
      }
      mGuards.clear();
   }
}