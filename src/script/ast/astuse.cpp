
#include "astuse.h"

#include "astvisitor.h"

ASTUse::ASTUse():
   ASTNode(),
   mIdentifier()
{
}

// get/set
   
const std::string& ASTUse::getIdentifier() const
{
   return mIdentifier;
}

void ASTUse::setIdentifier(const std::string& identifier)
{
   mIdentifier = identifier;
}

// - Visitor

ACCEPT_IMPL(ASTUse)
