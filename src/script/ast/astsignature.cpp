
#include "astsignature.h"

#include "core/smartptr/autoptr.h"
#include "core/defines.h"

#include "../ast/asttype.h"
#include "../ast/astclass.h"
#include "../ast/asttypevariable.h"
#include "../ast/asttypevariables.h"

ASTSignature::ASTSignature():
   mTypes(),
   mOwning(false) 
{
}

ASTSignature::~ASTSignature()
{
   if ( mOwning )
   {
      for ( std::size_t index = 0; index < mTypes.size(); index++ ) 
      {
         delete mTypes[index];
      }
      mTypes.clear(); 
   }
}

// - Query

const ASTType& ASTSignature::operator[](int index) const
{
   return *mTypes[index];
}

int ASTSignature::size() const
{
   return mTypes.size();
}

bool ASTSignature::exactMatch(const ASTSignature& that) const
{
   if ( that.size() != size() )
      return false;

   for ( std::size_t index = 0; index < mTypes.size(); index++ )
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

bool ASTSignature::bestMatch(const ASTSignature& that, const ASTTypeList& types) const
{
   if ( that.size() != size() )
      return false;

   for ( std::size_t index = 0; index < mTypes.size(); index++ )
   {
      AutoPtr<ASTType> ptype = mTypes[index]->clone();
      const ASTType& thattype = *that.mTypes[index];
      
      if ( ptype->isGeneric() && thattype.isGeneric() )
      {
         // both generic, so check if generic types are the same
      }
      else if ( ptype->isGeneric() && !thattype.isGeneric() )
      {
         if ( types.empty() )
         {
            // user probably forgot to specify the type arguments
            return false;
         }
         else if ( ptype->isArray() )
         {
            ptype->setArrayType(types[ptype->getTypeVariable().getIndex()].clone());
         }
         else
         {
            *ptype = (types[ptype->getTypeVariable().getIndex()]);
         }
      }

      if ( !thattype.greater(*ptype) )
      {
         return false;
      }
   }

   return true;
}

// - Operations

void ASTSignature::append(const ASTType& type)
{
   // ASSERT(!mOwning);
   mTypes.push_back(&type);
}

void ASTSignature::append(ASTType* ptype)
{
   mTypes.push_back(ptype);
   mOwning = true;
}

// conversion
   
String ASTSignature::toString() const
{
   String result;
   for ( std::size_t index = 0; index < mTypes.size(); index++ )
   {
      const ASTType* ptype = mTypes[index];
      result += ptype->toString();

      if ( index < mTypes.size() - 1 )
      {
         result += ", ";
      }
   }
   return result;
}