
#include "astuse.h"

#include "astvisitor.h"

ASTUse::ASTUse():
   ASTNode(),
   mIdentifier()
{
}

// get/set
   
const String& ASTUse::getIdentifier() const
{
   return mIdentifier;
}

void ASTUse::setIdentifier(const String& identifier)
{
   mIdentifier = identifier;
}

// - Visitor

ACCEPT_IMPL(ASTUse)
