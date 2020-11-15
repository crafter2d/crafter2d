
#include "astcatch.h"

#include "astblock.h"
#include "astlocalvariable.h"
#include "astvisitor.h"

ASTCatch::ASTCatch():
   ASTNode(),
   mpVariable(nullptr),
   mpBody(nullptr)
{
}

ASTCatch::~ASTCatch()
{
   setVariable(nullptr);
   setBody(nullptr);
}

// - Get/set

const ASTLocalVariable& ASTCatch::getVariable() const
{
   return *mpVariable;
}

ASTLocalVariable& ASTCatch::getVariable()
{
   return *mpVariable;
}

void ASTCatch::setVariable(ASTLocalVariable* pvariable)
{
   delete mpVariable;
   mpVariable = pvariable;
}

const ASTBlock& ASTCatch::getBody() const
{
   return *mpBody;
}

ASTBlock& ASTCatch::getBody()
{
   return *mpBody;
}

void ASTCatch::setBody(ASTBlock* pbody)
{
   delete mpBody;
   mpBody = pbody;
}

// - Visitor

ACCEPT_IMPL(ASTCatch)
