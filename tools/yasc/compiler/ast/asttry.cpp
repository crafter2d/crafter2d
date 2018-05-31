
#include "asttry.h"

#include "astblock.h"
#include "astcatch.h"
#include "astvisitor.h"
#include "astlocalvariable.h"
#include "astvariable.h"

ASTTry::ASTTry():
   ASTStatement(),
   mpBody(nullptr),
   mpFinally(nullptr),
   mResourceIndex(-1)
{
}

ASTTry::~ASTTry()
{
   setBody(nullptr);
   setFinallyBlock(nullptr);
}

// - Get/set

const ASTBlock& ASTTry::getBody() const
{
   return *mpBody;
}

ASTBlock& ASTTry::getBody()
{
   return *mpBody;
}

void ASTTry::setBody(ASTBlock* pbody)
{
   delete mpBody;
   mpBody = pbody;
}

bool ASTTry::hasFinallyBlock() const
{
   return mpFinally != nullptr;
}

const ASTBlock& ASTTry::getFinallyBlock() const
{
   return *mpFinally;
}

ASTBlock& ASTTry::getFinallyBlock()
{
   return *mpFinally;
}

void ASTTry::setFinallyBlock(ASTBlock* pblock)
{
   delete mpFinally;
   mpFinally = pblock;
}

int ASTTry::getResourceIndex() const
{
   return mResourceIndex;
}

void ASTTry::setResourceIndex(int resourceindex)
{
   mResourceIndex = resourceindex;

   // also set the resource index on the variables of the catch statements
   for ( int index = 0; index < getChildren().size(); index++ )
   {
      ASTCatch& c = static_cast<ASTCatch&>(getChildren()[index]);
      c.getVariable().getVariable().setResourceIndex(resourceindex);
   }
}

const ASTNodes& ASTTry::getCatches() const
{
   return getChildren();
}

// - Query
   
bool ASTTry::hasReturn(bool& hasunreachablecode) const
{
   if ( getBody().hasReturn(hasunreachablecode) )
   {
      const ASTNodes& catches = getChildren();
      for ( int index = 0; index < catches.size(); index++ )
      {
         const ASTCatch& astcatch = static_cast<const ASTCatch&>(catches[index]);
         if ( !astcatch.getBody().hasReturn(hasunreachablecode) )
         {
            return false;
         }
      }
      return true;
   }
   return false;
}

// - Operations
   
void ASTTry::addCatch(ASTCatch* pcatch)
{
   addChild(pcatch);
}

// - Visitor

ACCEPT_IMPL(ASTTry)
