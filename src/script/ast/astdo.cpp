
#include "astdo.h"

#include "astcompound.h"
#include "aststatement.h"
#include "astvisitor.h"

ASTDo::ASTDo():
   ASTStatement(),
   mpCondition(NULL),
   mpBody(NULL)
{
}

ASTDo::~ASTDo()
{
   setCondition(NULL);
   setBody(NULL);
}

// - Get/set
   
const ASTCompound& ASTDo::getCondition() const
{
   return *mpCondition;
}

ASTCompound& ASTDo::getCondition()
{
   return *mpCondition;
}

void ASTDo::setCondition(ASTCompound* pcondition)
{
   delete mpCondition;
   mpCondition = pcondition;
}

const ASTStatement& ASTDo::getBody() const
{
   return *mpBody;
}

ASTStatement& ASTDo::getBody()
{
   return *mpBody;
}

void ASTDo::setBody(ASTStatement* pbody)
{
   delete mpBody;
   mpBody = pbody;
}

// - Visit

ACCEPT_IMPL(ASTDo)
