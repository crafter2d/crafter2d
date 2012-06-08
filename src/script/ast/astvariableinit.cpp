
#include "astvariableinit.h"

#include "astarrayinit.h"
#include "astexpression.h"

ASTVariableInit::ASTVariableInit():
   mpExpression(NULL),
   mpArrayInit(NULL)
{
}

ASTVariableInit::~ASTVariableInit()
{
   setArrayInit(NULL);
   setExpression(NULL);
}

// - Get/set
bool ASTVariableInit::hasExpression() const
{
   return mpExpression != NULL;
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
   delete mpExpression;
   mpExpression = pexpression;
}

bool ASTVariableInit::hasArrayInit() const
{
   return mpArrayInit != NULL;
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
   delete mpArrayInit;
   mpArrayInit = parrayinit;
}
