
#ifndef AST_CATCH_H_
#define AST_CATCH_H_

#include "astnode.h"

class ASTBlock;
class ASTLocalVariable;

class ASTCatch : public ASTNode
{
public:
   ASTCatch();
   virtual ~ASTCatch();

 // get/set
   const ASTLocalVariable& getVariable() const;
         ASTLocalVariable& getVariable();
   void               setVariable(ASTLocalVariable* pvariable);

   const ASTBlock& getBody() const;
         ASTBlock& getBody();
   void            setBody(ASTBlock* pbody);
   
 // visitor
   ACCEPT;

private:
   ASTLocalVariable* mpVariable;
   ASTBlock*         mpBody;
};

#endif // AST_CATCH_H_
