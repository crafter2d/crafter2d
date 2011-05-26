
#ifndef AST_SWITCH_H_
#define AST_SWITCH_H_

#include "aststatement.h"

class CompileContext;
class ASTType;

class ASTSwitch : public ASTStatement
{
public:
   ASTSwitch();
   virtual ~ASTSwitch();

 // get/set
   const ASTNode& getExpression() const;
         ASTNode& getExpression();
   void           setExpression(ASTNode* pexpression);

 // query
   int getDefaultCount() const;

 // operations
   void validateCaseTypes(CompileContext& context, ASTType& expectedtype);

 // visitor
   ACCEPT;

private:
   ASTNode* mpExpression;
};

#endif // AST_SWITCH_H_
