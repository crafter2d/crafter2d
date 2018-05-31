
#include "astfor.h"

#include "astexpression.h"
#include "astlocalvariable.h"
#include "astvisitor.h"

ASTFor::ASTFor():
   mpInitialization(nullptr),
   mpCondition(nullptr),
   mpBody(nullptr)
{
}

ASTFor::~ASTFor()
{
   setInitializer(nullptr);
   setCondition(nullptr);
   setBody(nullptr);
}

// - Get/set

bool ASTFor::hasInitializer() const
{
   return mpInitialization != nullptr;
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
   return mpCondition != nullptr;
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
