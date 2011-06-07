
#ifndef PRELOAD_VISITOR_H_
#define PRELOAD_VISITOR_H_

#include <string>

#include "script/compiler/compilestep.h"
#include "script/compiler/classresolver.h"

class ASTType;
class CompileContext;

class PreloadVisitor : public CompileStep
{
public:
   PreloadVisitor(CompileContext& context);
   virtual ~PreloadVisitor();

 // visiting
   virtual void visit(ASTRoot& ast);
   virtual void visit(ASTPackage& ast);
   virtual void visit(ASTUse& ast);
   virtual void visit(ASTClass& ast);
   virtual void visit(ASTFunction& ast);
   virtual void visit(ASTFunctionArgument& ast);
   virtual void visit(ASTField& ast);
   virtual void visit(ASTBlock& ast);
   virtual void visit(ASTLocalVariable& ast);
   virtual void visit(ASTExpressionStatement& ast);
   virtual void visit(ASTIf& ast);
   virtual void visit(ASTFor& ast);
   virtual void visit(ASTForeach& ast);
   virtual void visit(ASTWhile& ast);
   virtual void visit(ASTDo& ast);
   virtual void visit(ASTSwitch& ast);
   virtual void visit(ASTCase& ast);
   virtual void visit(ASTReturn& ast);
   virtual void visit(ASTTry& ast);
   virtual void visit(ASTCatch& ast);
   virtual void visit(ASTThrow& ast);
   virtual void visit(ASTExpression& ast);
   virtual void visit(ASTConcatenate& ast);
   virtual void visit(ASTUnary& ast);
   virtual void visit(ASTNew& ast);
   virtual void visit(ASTCast& ast);
   virtual void visit(ASTAccess& ast);
   virtual void visit(ASTCompound& compound);

protected:
 // overloads
   virtual bool performStep(ASTNode& node);

private:
   bool load(ASTType& type);

   CompileContext&   mContext;
   ClassResolver     mClassResolver;
   std::string       mPackage;
};

#endif // PRELOAD_VISITOR_H_
