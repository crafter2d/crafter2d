
#include "astconcatenate.h"

#include "astvisitor.h"
#include "astnode.h"

ASTConcatenate::ASTConcatenate(Mode mode):
   ASTNode(),
   mMode(mode),
   mpLeft(nullptr),
   mpRight(nullptr)
{
}

ASTConcatenate::~ASTConcatenate()
{
   setLeft(nullptr);
   setRight(nullptr);
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
   mpLeft = nullptr;
   return pleft;
}

void ASTConcatenate::setLeft(ASTNode* pleft)
{
   delete mpLeft;
   mpLeft = pleft;
}

bool ASTConcatenate::hasRight() const
{
   return mpRight != nullptr;
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
   mpRight = nullptr;
   return pright;
}

void ASTConcatenate::setRight(ASTNode* pright)
{
   delete mpRight;
   mpRight = pright;
}

// - Operations

void ASTConcatenate::swapSides()
{
   ASTNode* pnode = mpLeft;
   mpLeft = mpRight;
   mpRight = pnode;
}

// - Visiting

ACCEPT_IMPL(ASTConcatenate)
