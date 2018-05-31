
#include "astvariableinit.h"

#include "astarrayinit.h"
#include "astexpression.h"
#include "astvisitor.h"

ASTVariableInit::ASTVariableInit():
   ASTNode(),
   mpExpression(nullptr),
   mpArrayInit(nullptr)
{
}

ASTVariableInit::~ASTVariableInit()
{
}

// - Get/set
bool ASTVariableInit::hasExpression() const
{
   return mpExpression != nullptr;
}

const ASTExpression& ASTVariableInit::getExpression() const
{
   return *mpExpression;
}

ASTExpression& ASTVariableInit::getExpression()
{
   return *mpExpression;
}

void ASTVariableInit::setExpression(ASTExpression* pexpression)
{
   mpExpression = pexpression;
   addChild(pexpression);
}

bool ASTVariableInit::hasArrayInit() const
{
   return mpArrayInit != nullptr;
}

const ASTArrayInit& ASTVariableInit::getArrayInit() const
{
   return *mpArrayInit;
}

ASTArrayInit& ASTVariableInit::getArrayInit()
{
   return *mpArrayInit;
}

void ASTVariableInit::setArrayInit(ASTArrayInit* parrayinit)
{
   mpArrayInit = parrayinit;
   addChild(parrayinit);
}

// - Visitor

ACCEPT_IMPL(ASTVariableInit);
