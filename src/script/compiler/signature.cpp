
#include "signature.h"

#include "../ast/asttype.h"
#include "../ast/asttypevariable.h"
#include "../ast/asttypevariables.h"

Signature::Signature():
   mTypes(),
   mOwning(false) 
{
}

Signature::~Signature()
{
   if ( mOwning )
   {
      for ( int index = 0; index < mTypes.size(); index++ ) 
      {
         delete mTypes[index];
      }
      mTypes.clear(); 
   }
}

// - Query

const ASTType& Signature::operator[](int index) const
{
   return *mTypes[index];
}

int Signature::size() const
{
   return mTypes.size();
}

bool Signature::exactMatch(const Signature& that) const
{
   if ( that.size() != size() )
      return false;

   for ( int index = 0; index < mTypes.size(); index++ )
   {
      const ASTType& type = *mTypes[index];
      const ASTType& thattype = *that.mTypes[index];

      if ( !type.equals(thattype) )
      {
         return false;
      }
   }

   return true;
}

bool Signature::bestMatch(const Signature& that, const ASTTypeList& types) const
{
   if ( that.size() != size() )
      return false;

   for ( int index = 0; index < mTypes.size(); index++ )
   {
      const ASTType* ptype = mTypes[index];
      const ASTType& thattype = *that.mTypes[index];
      
      if ( ptype->isGeneric() )
      {
         ptype = &types[ptype->getTypeVariable().getIndex()];
      }

      if ( !thattype.greater(*ptype) )
      {
         return false;
      }
   }

   return true;
}

// - Operations

void Signature::append(const ASTType& type)
{
   // Q_ASSERT(!mOwning);
   mTypes.push_back(&type);
}

void Signature::append(ASTType* ptype)
{
   mTypes.push_back(ptype);
   mOwning = true;
}
