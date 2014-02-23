
#ifndef COMPILE_STEP_H_
#define COMPILE_STEP_H_

#include "ast/astvisitor.h"

class ASTNode;

class CompileStep : public ASTVisitor
{
public:
   CompileStep();

 // operations
   bool step(ASTNode& node);

protected:
   virtual bool performStep(ASTNode& node) = 0;
};

#endif // COMPILE_STEP_H_
