
#include "astnew.h"

#include "astexpression.h"
#include "asttype.h"
#include "astvisitor.h"

ASTNew::ASTNew():
   ASTExpressionPart(),
   mKind(eInvalid),
   mpType(NULL),
   mpConstructor(NULL)
{
}

ASTNew::~ASTNew()
{
   setType(NULL);
}

// - Get/set

ASTNew::Kind ASTNew::getKind() const
{
   return mKind;
}
   
void ASTNew::setKind(Kind kind)
{
   mKind = kind;
}

const ASTType& ASTNew::getType() const
{
   return *mpType;
}

ASTType& ASTNew::getType()
{
   return *mpType;
}

void ASTNew::setType(ASTType* ptype)
{
   delete mpType;
   mpType = ptype;
}

const ASTFunction& ASTNew::getConstructor() const
{
   return *mpConstructor;
}

void ASTNew::setConstructor(const ASTFunction& constructor)
{
   mpConstructor = &constructor;
}

const ASTNodes& ASTNew::getArguments() const
{
   return getChildren();
}

ASTNodes& ASTNew::getArguments()
{
   return getChildren();
}

// - Query

int ASTNew::getArgumentCount() const
{
   return getChildren().size();
}

int ASTNew::getArrayDimension() const
{
   return getChildren().size();
}

// - Operations

void ASTNew::addArgument(ASTExpression* pexpression)
{
   addChild(pexpression);
}

// - Visitor

ACCEPT_IMPL(ASTNew)
