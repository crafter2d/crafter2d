
#include "types.h"

#include "type.h"

namespace yasc
{
   Types::Types() :
      mTypes()
   {
   }

   Types::~Types()
   {
      clear();
   }

   const Types& Types::operator=(const Types& that)
   {
      for ( std::size_t index = 0; index < that.mTypes.size(); ++index )
      {
         mTypes.push_back(that.mTypes[index]->clone());
      }
      return *this;
   }

   const Type& Types::operator[](int index) const
   {
      return *mTypes[index];
   }

   // - Query

   int Types::size() const
   {
      return mTypes.size();
   }

   bool Types::equals(const Types& that) const
   {
      if ( size() != that.size() )
      {
         return false;
      }

      for ( std::size_t index = 0; index < mTypes.size(); ++index )
      {
         if ( !mTypes[index]->equals(*that.mTypes[index]) )
         {
            return false;
         }
      }

      return true;
   }

   bool Types::compareSignature(bool isstatic, const Types& that) const
   {
      std::size_t cnt = size() - (isstatic ? 0 : 1);
      if ( cnt != that.size() )
      {
         return false;
      }

      for ( std::size_t index = 0; index < cnt; ++index )
      {
         if ( !mTypes[index + 1]->equals(*that.mTypes[index]) )
         {
            return false;
         }
      }

      return true;
   }

   // - Operations

   void Types::add(Type* ptype)
   {
      mTypes.push_back(ptype);
   }

   void Types::clear()
   {
      for ( std::size_t index = 0; index < mTypes.size(); ++index )
      {
         delete mTypes[index];
      }
      mTypes.clear();
   }
}
