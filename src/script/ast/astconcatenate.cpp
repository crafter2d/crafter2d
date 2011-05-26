
#include "astconcatenate.h"

#include "astvisitor.h"
#include "astnode.h"

ASTConcatenate::ASTConcatenate(Mode mode):
   ASTNode(),
   mMode(mode),
   mpLeft(NULL),
   mpRight(NULL)
{
}

ASTConcatenate::~ASTConcatenate()
{
   setLeft(NULL);
   setRight(NULL);
}

// - Get/set

ASTConcatenate::Mode ASTConcatenate::getMode() const
{
   return mMode;
}

const ASTNode& ASTConcatenate::getLeft() const
{
   return *mpLeft;
}

ASTNode& ASTConcatenate::getLeft()
{
   return *mpLeft;
}

ASTNode* ASTConcatenate::useLeft()
{
   ASTNode* pleft = mpLeft;
   mpLeft = NULL;
   return pleft;
}

void ASTConcatenate::setLeft(ASTNode* pleft)
{
   delete mpLeft;
   mpLeft = pleft;
}

bool ASTConcatenate::hasRight() const
{
   return mpRight != NULL;
}

const ASTNode& ASTConcatenate::getRight() const
{
   return *mpRight;
}

ASTNode& ASTConcatenate::getRight()
{
   return *mpRight;
}

ASTNode* ASTConcatenate::useRight()
{
   ASTNode* pright = mpRight;
   mpRight = NULL;
   return pright;
}

void ASTConcatenate::setRight(ASTNode* pright)
{
   delete mpRight;
   mpRight = pright;
}

// - Visiting

ACCEPT_IMPL(ASTConcatenate)
