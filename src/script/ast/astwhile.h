
#ifndef AST_WHILE_H_
#define AST_WHILE_H_

#include "aststatement.h"

class ASTCompound;
class ASTStatement;

class ASTWhile : public ASTStatement
{
public:
   ASTWhile();
   virtual ~ASTWhile();

 // get/set
   const ASTCompound& getCondition() const;
         ASTCompound& getCondition();
   void               setCondition(ASTCompound* pexpression);

   const ASTStatement& getBody() const;
         ASTStatement& getBody();
   void                setBody(ASTStatement* pbody);

 // visitor
   ACCEPT;

private:
   ASTCompound* mpCondition;
   ASTStatement*  mpBody;
};

#endif // AST_WHILE_H_
