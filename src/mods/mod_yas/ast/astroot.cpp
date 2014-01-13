
#include "astroot.h"

#include "astvisitor.h"
#include "astclass.h"

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

void ASTRoot::detachClasses()
{
   ASTNodes& nodes = getChildren();
   for ( int index = 0; index < nodes.size(); index++ )
   {
      const ASTNode& node = nodes[index];
      if ( dynamic_cast<const ASTClass*>(&node) != NULL )
      {
         nodes.detach(node);
         index--;
      }
   }
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
