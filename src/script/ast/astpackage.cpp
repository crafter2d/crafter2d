
#include "astpackage.h"

#include "astvisitor.h"

ASTPackage::ASTPackage():
   ASTNode(),
   mName()
{
}

// - Get/set

const String& ASTPackage::getName() const 
{ 
   return mName;
}
   
void ASTPackage::setName(const String& name)
{
   mName = name;
}

// - Visitor

ACCEPT_IMPL(ASTPackage);
