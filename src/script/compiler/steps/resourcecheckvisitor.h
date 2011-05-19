
#ifndef RESOURCE_CHECK_VISITOR_H_
#define RESOURCE_CHECK_VISITOR_H_

#include "../compilestep.h"

class CompileContext;

class ResourceCheckVisitor : public CompileStep
{
public:
   ResourceCheckVisitor(CompileContext& context);

 // visitor
   virtual void visit(ASTRoot& ast);
   virtual void visit(ASTClass& ast);
   virtual void visit(ASTFunction& ast);
   virtual void visit(ASTBlock& ast);
   virtual void visit(ASTLocalVariable& ast);
   virtual void visit(ASTIf& ast);
   virtual void visit(ASTFor& ast);
   virtual void visit(ASTForeach& ast);
   virtual void visit(ASTWhile& ast);
   virtual void visit(ASTDo& ast);
   virtual void visit(ASTSwitch& ast);
   virtual void visit(ASTCase& ast);
   virtual void visit(ASTTry& ast);
   virtual void visit(ASTCatch& ast);

protected:
 // overloads
   virtual bool performStep(ASTNode& node);

private:
   CompileContext& mContext;
   int             mOffset;
   int             mMaxVariables;
};

#endif // RESOURCE_CHECK_VISITOR_H_
