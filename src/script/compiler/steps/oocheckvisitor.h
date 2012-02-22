
#ifndef OOCHECK_VISITOR_H_
#define OOCHECK_VISITOR_H_

#include "../compilestep.h"

#include "script/scope/scopestack.h"

class ASTVariable;
class ASTType;
class CompileContext;

class OOCheckVisitor : public CompileStep
{
public:
   explicit OOCheckVisitor(CompileContext& context);

 // visitor
   virtual void visit(ASTRoot& ast);
   virtual void visit(ASTClass& ast);
   virtual void visit(ASTFunction& ast);
   virtual void visit(ASTFunctionArgument& ast);
   virtual void visit(ASTBlock& ast);
   virtual void visit(ASTExpressionStatement& ast);
   virtual void visit(ASTIf& ast);
   virtual void visit(ASTFor& ast);
   virtual void visit(ASTForeach& ast);
   virtual void visit(ASTWhile& ast);
   virtual void visit(ASTDo& ast);
   virtual void visit(ASTSwitch& ast);
   virtual void visit(ASTCase& ast);
   virtual void visit(ASTReturn& ast);
   virtual void visit(ASTAssert& ast);
   virtual void visit(ASTExpression& ast);
   virtual void visit(ASTConcatenate& ast);
   virtual void visit(ASTUnary& ast);
   virtual void visit(ASTSuper& ast);
   virtual void visit(ASTNew& ast);
   virtual void visit(ASTAccess& ast);
   virtual void visit(ASTCompound& compound);

protected:
 // overloads
   virtual bool performStep(ASTNode& node);

private:
 // operations
   bool isFinal(ASTNode& expr);
   bool hasSuperCall(const ASTFunction& function) const;

   void validateClass(const ASTClass& aclass);
   void validateNullConcatenate(ASTConcatenate& concatenate, const ASTType& left, const ASTType& right);

   CompileContext&      mContext;
   ASTClass*            mpClass;
   const ASTVariable*   mpVariable;
   const ASTType*       mpCurrentType;
   ScopeStack           mScopeStack;
};

#endif // OOCHECK_VISITOR_H_
