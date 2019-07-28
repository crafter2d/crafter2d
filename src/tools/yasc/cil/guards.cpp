
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
         const Guard& guard = that[index];
         mGuards.push_back(guard.clone());
      }
      return *this;
   }

   const Guard& Guards::operator[](int index) const
   {
      return *mGuards[index];
   }

   Guard& Guards::operator[](int index)
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

   // - Search

   const Guard* Guards::find(int instruction) const
   {
      for ( std::size_t index = 0; index < mGuards.size(); ++index )
      {
         const Guard* pguard = mGuards[Guard::sStart];
         if ( pguard->labels[0] == instruction )
         {
            return pguard;
         }
      }
      return nullptr;
   }
}
