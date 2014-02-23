
#ifndef AST_DO_H_
#define AST_DO_H_

#include "aststatement.h"

class ASTCompound;
class ASTStatement;

class ASTDo : public ASTStatement
{
public:
   ASTDo();
   virtual ~ASTDo();

 // get/set
   const ASTCompound& getCondition() const;
         ASTCompound& getCondition();
   void               setCondition(ASTCompound* pexpression);

   const ASTStatement& getBody() const;
         ASTStatement& getBody();
   void                setBody(ASTStatement* pbody);

 // query
   virtual bool hasReturn(bool& hasunreachablecode) const;

 // visit
   ACCEPT;

private:
   ASTCompound*   mpCondition;
   ASTStatement*  mpBody;
};

#endif // AST_DO_H_
