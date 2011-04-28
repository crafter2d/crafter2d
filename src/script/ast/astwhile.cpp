
#include "astwhile.h"

#include "astcompound.h"
#include "aststatement.h"
#include "astvisitor.h"

ASTWhile::ASTWhile():
   ASTStatement(),
   mpCondition(NULL),
   mpBody(NULL)
{
}

ASTWhile::~ASTWhile()
{
   setCondition(NULL);
   setBody(NULL);
}

// - Get/set
   
const ASTCompound& ASTWhile::getCondition() const
{
   return *mpCondition;
}

ASTCompound& ASTWhile::getCondition()
{
   return *mpCondition;
}

void ASTWhile::setCondition(ASTCompound* pcondition)
{
   delete mpCondition;
   mpCondition = pcondition;
}

const ASTStatement& ASTWhile::getBody() const
{
   return *mpBody;
}

ASTStatement& ASTWhile::getBody()
{
   return *mpBody;
}

void ASTWhile::setBody(ASTStatement* pbody)
{
   delete mpBody;
   mpBody = pbody;
}

// - Visitor

ACCEPT_IMPL(ASTWhile)
