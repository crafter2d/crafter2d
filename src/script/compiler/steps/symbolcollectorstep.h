
#ifndef SYMBOL_COLLECTOR_VISITOR_H_
#define SYMBOL_COLLECTOR_VISITOR_H_

#include "../compilestep.h"
#include "../classresolver.h"

#include <vector>

class ASTClass;
class CompileContext;

class SymbolCollectorVisitor : public CompileStep
{
public:
   explicit SymbolCollectorVisitor(CompileContext& context);
   virtual ~SymbolCollectorVisitor();
   
 // visitor
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
   typedef std::vector<std::string> Paths;

 // operations
   void createDefaultConstructor(ASTClass& ast);

   CompileContext&  mContext;
   std::string      mPackage;
   ClassResolver    mResolver;
   ASTClass*        mpClass;
};

#endif // SYMBOL_COLLECTOR_VISITOR_H_
