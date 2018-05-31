
#include "astexpression.h"

#include "astvisitor.h"

ASTExpression::ASTExpression():
   ASTNode(),
   mpLeft(nullptr),
   mpRight(nullptr),
   mKind(eInvalid)
{
}

ASTExpression::~ASTExpression()
{
   setLeft(nullptr);
   setRight(nullptr);
}

// - Get/set
   
ASTExpression::Kind ASTExpression::getKind() const
{
   return mKind;
}
   
void ASTExpression::setKind(Kind kind)
{
   mKind = kind;
}

const ASTNode& ASTExpression::getLeft() const
{
   return *mpLeft;
}

ASTNode& ASTExpression::getLeft()
{
   return *mpLeft;
}

ASTNode* ASTExpression::useLeft()
{
   ASTNode* pleft = mpLeft;
   mpLeft = nullptr;
   return pleft;
}

void ASTExpression::setLeft(ASTNode* pleft)
{
   delete mpLeft;
   mpLeft = pleft;
}

bool ASTExpression::hasRight() const
{
   return mpRight != nullptr;
}

const ASTExpression& ASTExpression::getRight() const
{
   return *mpRight;
}

ASTExpression& ASTExpression::getRight()
{
   return *mpRight;
}

ASTExpression* ASTExpression::useRight()
{
   ASTExpression* pright = mpRight;
   mpRight = nullptr;
   return pright;
}

void ASTExpression::setRight(ASTExpression* pright)
{
   delete mpRight;
   mpRight = pright;
}

// - Visitor
   
ACCEPT_IMPL(ASTExpression)
