
#include "astannotations.h" 

#include "astannotation.h"
#include "astvisitor.h"

ASTAnnotations::ASTAnnotations():
   ASTNode(),
   mAnnotations()
{
}

const ASTAnnotation& ASTAnnotations::operator[](int index) const
{
   return *mAnnotations[index];
}

// - Query

int ASTAnnotations::size() const
{
   return mAnnotations.size();
}

bool ASTAnnotations::contains(const std::string& name) const
{
   for ( std::size_t index = 0; index < mAnnotations.size(); index++ )
   {
      ASTAnnotation* pannotation = mAnnotations[index];
      if ( pannotation->mName == name )
      {
         return true;
      }
   }
   return false;
}

// - Operations
   
void ASTAnnotations::add(ASTAnnotation* pannotation)
{
   mAnnotations.push_back(pannotation);
}

// - Visitor

ACCEPT_IMPL(ASTAnnotations)
