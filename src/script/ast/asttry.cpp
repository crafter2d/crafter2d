
#include "asttry.h"

#include "astblock.h"
#include "astcatch.h"
#include "astvisitor.h"

ASTTry::ASTTry():
   ASTStatement(),
   mpBody(NULL),
   mpFinally(NULL),
   mResourceIndex(-1)
{
}

ASTTry::~ASTTry()
{
   setBody(NULL);
   setFinallyBlock(NULL);
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
   return mpFinally != NULL;
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

void ASTTry::setResourceIndex(int index)
{
   mResourceIndex = index;
}

const ASTNodes& ASTTry::getCatches() const
{
   return getChildren();
}

// - Operations
   
void ASTTry::addCatch(ASTCatch* pcatch)
{
   addChild(pcatch);
}

// - Visitor

ACCEPT_IMPL(ASTTry)
