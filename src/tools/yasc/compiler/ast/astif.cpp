
#include "astif.h"

#include "astcompound.h"
#include "aststatement.h"
#include "astvisitor.h"

ASTIf::ASTIf():
   ASTStatement(),
   mpCondition(nullptr),
   mpStatement(nullptr),
   mpElseStatement(nullptr)
{
}

ASTIf::~ASTIf()
{
   setCondition(nullptr);
   setStatement(nullptr);
   setElseStatement(nullptr);
}

// - Get/set

const ASTCompound& ASTIf::getCondition() const
{
   return *mpCondition;
}

ASTCompound& ASTIf::getCondition()
{
   return *mpCondition;
}

void ASTIf::setCondition(ASTCompound* pcondition)
{
   delete mpCondition;
   mpCondition = pcondition;
}

const ASTStatement& ASTIf::getStatement() const
{
   return *mpStatement;
}
         
ASTStatement& ASTIf::getStatement()
{
   return *mpStatement;
}

void ASTIf::setStatement(ASTStatement* pstatement)
{
   delete mpStatement;
   mpStatement = pstatement;
}

bool ASTIf::hasElseStatement() const
{
   return mpElseStatement != nullptr;
}

const ASTStatement& ASTIf::getElseStatement() const
{
   return *mpElseStatement;
}

ASTStatement& ASTIf::getElseStatement()
{
   return *mpElseStatement;
}

void ASTIf::setElseStatement(ASTStatement* pstatement)
{
   delete mpElseStatement;
   mpElseStatement = pstatement;
}

// - Query

bool ASTIf::hasReturn(bool& hasunreachablecode) const
{
   if ( hasElseStatement() )
   {
      return mpStatement->hasReturn(hasunreachablecode) && mpElseStatement->hasReturn(hasunreachablecode);
   }
   return false;
}

// - Visitor

ACCEPT_IMPL(ASTIf)
