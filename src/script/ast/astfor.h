
#ifndef AST_FOR_H_
#define AST_FOR_H_

#include "aststatement.h"

class ASTLocalVariable;
class ASTExpression;
class ASTStatement;

class ASTFor : public ASTStatement
{
public:
   ASTFor();
   virtual ~ASTFor();

 // get/set
   bool                    hasInitializer() const;
   const ASTLocalVariable& getInitializer() const;
         ASTLocalVariable& getInitializer();
   void                    setInitializer(ASTLocalVariable* pvariable);

   bool                 hasCondition() const;
   const ASTExpression& getCondition() const;
         ASTExpression& getCondition();
   void                 setCondition(ASTExpression* pexpression);

   const ASTStatement& getBody() const;
         ASTStatement& getBody();
   void                setBody(ASTStatement* pbody);
   
 // operations
   void addLoop(ASTExpression* pexpression);
   
 // visitor
   ACCEPT;

private:
   ASTLocalVariable* mpInitialization;
   ASTExpression*    mpCondition;
   ASTStatement*     mpBody;
};

#endif // AST_FOR_H_
