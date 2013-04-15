
#include "signature.h"

namespace CIL
{

   const Type& Signature::operator[](int index) const
   {
      return *mArguments[index];
   }

   // - Query

   int Signature::size() const
   {
      return mArguments.size();
   }

   bool Signature::match(const Signature& that) const
   {
      if ( mArguments.size() == that.mArguments.size() )
      {
         for ( unsigned index = 0; index < mArguments.size(); ++index )
         {
            const Type* ptype = mArguments[index];
            const Type* pthattype = that.mArguments[index];

            if ( !ptype->match(*pthattype) )
            {
               return false;
            }
         }

         return true;
      }

      return false;
   }

   // - Operations

   void Signature::addArgument(Type* ptype)
   {
      mArguments.push_back(ptype);
   }

} // namespace CIL