
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

void ASTRoot::collectClasses(std::vector<ASTClass*>& classes)
{
   ASTNodes& nodes = getChildren();
   for ( int index = 0; index < nodes.size(); index++ )
   {
      ASTNode& node = nodes[index];
      ASTClass* pclass = dynamic_cast<ASTClass*>(&node);
      if ( pclass != NULL )
      {
         classes.push_back(pclass);
      }
   }
}

// - Visitor

ACCEPT_IMPL(ASTRoot)
