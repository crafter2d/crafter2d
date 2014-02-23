
#ifndef AST_IF_H_
#define AST_IF_H_

#include "aststatement.h"

class ASTCompound;
class ASTStatement;

class ASTIf : public ASTStatement
{
public:
   ASTIf();
   virtual ~ASTIf();

 // get/set
   const ASTCompound& getCondition() const;
         ASTCompound& getCondition();
   void               setCondition(ASTCompound* pcondition);

   const ASTStatement& getStatement() const;
         ASTStatement& getStatement();
   void                setStatement(ASTStatement* pstatement);

   bool                hasElseStatement() const;
   const ASTStatement& getElseStatement() const;
         ASTStatement& getElseStatement();
   void setElseStatement(ASTStatement* pstatement);

 // query
   virtual bool hasReturn(bool& hasunreachablecode) const;

 // visitor
   ACCEPT;

private:
   ASTCompound*  mpCondition;
   ASTStatement* mpStatement;
   ASTStatement* mpElseStatement;
};

#endif // AST_IF_H_
