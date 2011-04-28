
#ifndef SYMBOL_CHECK_VISITOR_H_
#define SYMBOL_CHECK_VISITOR_H_

#include "script/compiler/compilestep.h"

#include "script/ast/asttype.h"
#include "script/ast/astunary.h"
#include "script/scope/scopestack.h"

class CompileContext;
class ASTAccess;
class ASTClass;
class ASTClassTable;
class ASTLiteralTable;
class ASTFunctionBase;

class SymbolCheckVisitor : public CompileStep
{
public:
   SymbolCheckVisitor(CompileContext& context);
   
 // visiting
   virtual void visit(ASTRoot& root);
   virtual void visit(ASTClass& ast);
   virtual void visit(ASTFunction& ast);
   virtual void visit(ASTFunctionArgument& ast);
   virtual void visit(ASTField& ast);
   virtual void visit(ASTBlock& ast);
   virtual void visit(ASTExpressionStatement& ast);
   virtual void visit(ASTLocalVariable& ast);
   virtual void visit(ASTIf& ast);
   virtual void visit(ASTFor& ast);
   virtual void visit(ASTForeach& ast);
   virtual void visit(ASTWhile& ast);
   virtual void visit(ASTDo& ast);
   virtual void visit(ASTReturn& ast);
   virtual void visit(ASTTry& ast);
   virtual void visit(ASTCatch& ast);
   virtual void visit(ASTThrow& ast);
   virtual void visit(ASTExpression& ast);
   virtual void visit(ASTConcatenate& ast);
   virtual void visit(ASTUnary& ast);
   virtual void visit(ASTNew& ast);
   virtual void visit(ASTSuper& ast);
   virtual void visit(ASTLiteral& ast);
   virtual void visit(ASTCast& ast);
   virtual void visit(ASTAccess& ast);
   virtual void visit(ASTCompound& compound);

protected:
 // overloads
   virtual bool performStep(ASTRoot& root);

private:
 // query
   bool isVariable(const ASTNode& node) const;

 // operations
   void checkFunction(const ASTClass& aclass, ASTAccess& access, bool isstatic);
   void checkOperator(ASTUnary::Operator op);

   CompileContext& mContext;
   ASTClass*       mpClass;
   ASTFunction*    mpFunction;
   ASTType         mCurrentType;
   ScopeStack      mScopeStack;
   bool            mStatic;
};
#endif // SYMBOL_CHECK_VISITOR_H_