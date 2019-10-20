
#include "types.h"

#include "type.h"

namespace yasc
{
   Types::Types() :
      mTypes()
   {
   }

   Types::Types(const Types& that) :
      mTypes(that.mTypes)
   {
   }

   Types::~Types()
   {
   }

   const Types& Types::operator=(const Types& that)
   {
      for ( std::size_t index = 0; index < that.mTypes.size(); ++index )
      {
         mTypes.push_back(that.mTypes[index]);
      }
      return *this;
   }

   const Type& Types::operator[](int index) const
   {
      return mTypes[index];
   }

   // - Query

   int Types::size() const
   {
      return mTypes.size();
   }

   bool Types::equals(const Types& that) const
   {
      return mTypes == that.mTypes;
   }

   bool Types::compareSignature(bool isstatic, const Types& that) const
   {
      std::size_t cnt = size() - (isstatic ? 0 : 1);
      if ( cnt != that.size() )
      {
         return false;
      }

      return std::equal(mTypes.begin() + 1, mTypes.end(), that.mTypes.begin());
   }

   // - Operations

   void Types::add(const Type& type)
   {
      mTypes.push_back(type);
   }

   void Types::add(Type&& type)
   {
      mTypes.push_back(std::move(type));
   }

   void Types::clear()
   {
      mTypes.clear();
   }
}
