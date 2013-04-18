
#ifndef BYTECODE_GENERATION_VISITOR_H
#define BYTECODE_GENERATION_VISITOR_H

#include "script/compiler/compilestep.h"

class CompileContext;
class VirtualClass;

class ByteCodeGenerationVisitor : public CompileStep
{
public:
   ByteCodeGenerationVisitor(CompileContext& context);

 // visitors 
   virtual void visit(const ASTRoot& root);
   virtual void visit(const ASTClass& ast);
   virtual void visit(const ASTFunction& ast);

protected:
 // overloads
   virtual bool performStep(ASTNode& node);

private:
   CompileContext& mContext;
   VirtualClass*   mpVirClass;
   char*           mpCode;
   int             mCodeSize;
};

#endif // BYTECODE_GENERATION_VISITOR_H
