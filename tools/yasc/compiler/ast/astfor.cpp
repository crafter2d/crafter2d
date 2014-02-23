
#include "astfor.h"

#include "astexpression.h"
#include "astlocalvariable.h"
#include "astvisitor.h"

ASTFor::ASTFor():
   mpInitialization(NULL),
   mpCondition(NULL),
   mpBody(NULL)
{
}

ASTFor::~ASTFor()
{
   setInitializer(NULL);
   setCondition(NULL);
   setBody(NULL);
}

// - Get/set

bool ASTFor::hasInitializer() const
{
   return mpInitialization != NULL;
}

const ASTLocalVariable& ASTFor::getInitializer() const
{
   return *mpInitialization;
}
         
ASTLocalVariable& ASTFor::getInitializer()
{
   return *mpInitialization;
}
   
void ASTFor::setInitializer(ASTLocalVariable* pvariable)
{
   delete mpInitialization;
   mpInitialization = pvariable;
}

bool ASTFor::hasCondition() const
{
   return mpCondition != NULL;
}

const ASTExpression& ASTFor::getCondition() const
{
   return *mpCondition;
}

ASTExpression& ASTFor::getCondition()
{
   return *mpCondition;
}

void ASTFor::setCondition(ASTExpression* pexpression)
{
   delete mpCondition;
   mpCondition = pexpression;
}

const ASTStatement& ASTFor::getBody() const
{
   return *mpBody;
}

ASTStatement& ASTFor::getBody()
{
   return *mpBody;
}

void ASTFor::setBody(ASTStatement* pbody)
{
   delete mpBody;
   mpBody = pbody;
}

// - Query
   
bool ASTFor::hasReturn(bool& hasunreachablecode) const
{
   return getBody().hasReturn(hasunreachablecode);
}

// - Operations

void ASTFor::addLoop(ASTExpression* pexpression)
{
   addChild(pexpression);
}

// - Visitor

ACCEPT_IMPL(ASTFor)
