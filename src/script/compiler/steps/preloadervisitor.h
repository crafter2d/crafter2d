
#ifndef PRELOADER_VISITOR_H_
#define PRELOADER_VISITOR_H_

#include "../compilestep.h"

#include "script/ast/ast.h"

#include "script/compiler/compiler.h"
#include "script/compiler/compilecontext.h"

class PreloaderVisitor : public CompileStep
{
public:
   PreloaderVisitor(Compiler& compiler):
      CompileStep(),
      mCompiler(compiler)
   {
   }

   // - Visit

   virtual void visit(const ASTRoot& root) {
      visitChildren(root);
   }

   virtual void visit(const ASTUse& use) {
      mCompiler.compile(use.getIdentifier());
   }

protected:
 // - Overloads
   virtual bool performStep(ASTRoot& root) {
      visit(root);
      return true;
   }

private:
   Compiler& mCompiler;
};


#endif // PRELOADER_VISITOR_H_
