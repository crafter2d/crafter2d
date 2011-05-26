
#include "astroot.h"

#include "astvisitor.h"

ASTRoot::ASTRoot():
   ASTNode(),
   mUses()
{
}

// - Get/set

const ASTRoot::StringList& ASTRoot::getUses() const
{
   return mUses;
}

// - Operations

void ASTRoot::addUse(const std::string& use)
{
   mUses.push_back(use);
}

// - Visitor

ACCEPT_IMPL(ASTRoot)

   /* 
   void ASTRoot::accept(ASTVisitor& visitor) const 
   { 
      visitor.visit(*this); 
   }

   void ASTRoot::accept(ASTVisitor& visitor)
   {
      visitor.visit(*this);
   }
   */
