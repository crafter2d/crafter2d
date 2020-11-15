
#ifndef VARIABLE_CHECK_VISITOR_H_
#define VARIABLE_CHECK_VISITOR_H_

#include "libyasc/ast/astvisitor.h"

class VariableCheckVisitor : public ASTVisitor
{
public:
   static bool isVariable(const ASTNode& node);

   VariableCheckVisitor();

 // visitor
   virtual void visit(const ASTExpression& ast);
   virtual void visit(const ASTUnary& ast);
   virtual void visit(const ASTCast& ast);
   virtual void visit(const ASTAccess& ast);

private:
   bool isVariableImpl(const ASTNode& node);

   bool mVariable;
};

#endif // VARIABLE_CHECK_VISITOR_H_
