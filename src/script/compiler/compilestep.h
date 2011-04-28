
#ifndef COMPILE_STEP_H_
#define COMPILE_STEP_H_

#include "../ast/astvisitor.h"

class ASTRoot;
class QStringList;

class CompileStep : public ASTVisitor
{
public:
   CompileStep();

 // operations
   bool step(ASTRoot& root);

protected:
   virtual bool performStep(ASTRoot& root) = 0;
};

#endif // COMPILE_STEP_H_
