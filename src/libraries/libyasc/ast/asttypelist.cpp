
#include "asttypelist.h"

#include "asttype.h"

ASTTypeList::ASTTypeList(const ASTTypeList& that):
   mTypeList()
{
   append(that);
}

ASTTypeList::ASTTypeList(ASTTypeList&& that):
   mTypeList(that.mTypeList.begin(), that.mTypeList.end())
{
   that.mTypeList.clear();
}

const ASTTypeList& ASTTypeList::operator=(const ASTTypeList& that)
{
   clear();
   append(that);
   return *this;
}

bool ASTTypeList::operator==(const ASTTypeList& that) const
{
   if ( size() == that.size() )
   {
      for ( std::size_t index = 0; index < mTypeList.size(); index++ )
      {
         const ASTType* ptype = mTypeList[index];
         const ASTType& thattype = that[index];

         if ( !ptype->equals(thattype) )
         {
            return false;
         }
      }

      return true;
   }

   return false;
}

// - Query

bool ASTTypeList::empty() const
{
   return size() == 0;
}

bool ASTTypeList::contains(const ASTClass& intrface) const
{
   for ( std::size_t index = 0; index < mTypeList.size(); index++ )
   {
      const ASTType* ptype = mTypeList.at(index);
      if ( ptype->hasObjectClass() && (&ptype->getObjectClass() == &intrface) )
      {
         return true;
      }
   }

   return false;
}

// - Operations

void ASTTypeList::append(const ASTTypeList& that)
{
   for ( int index = 0; index < that.size(); index++ )
   {
      mTypeList.push_back(that[index].clone());
   }
}

void ASTTypeList::clear()
{
   for ( std::size_t index = 0; index < mTypeList.size(); index++ )
   {
      delete mTypeList[index];
   }
   mTypeList.clear();
}