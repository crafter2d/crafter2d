
#include "astpackage.h"

#include "astvisitor.h"

ASTPackage::ASTPackage():
   ASTNode(),
   mName()
{
}

// - Get/set

const std::string& ASTPackage::getName() const 
{ 
   return mName;
}
   
void ASTPackage::setName(const std::string& name)
{
   mName = name;
}

// - Visitor

ACCEPT_IMPL(ASTPackage);
