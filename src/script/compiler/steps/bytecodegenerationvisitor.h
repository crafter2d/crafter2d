
#ifndef BYTECODE_GENERATION_VISITOR_H
#define BYTECODE_GENERATION_VISITOR_H

#include "script/compiler/compilestep.h"

class CompileContext;

class ByteCodeGenerationVisitor : public CompileStep
{
public:
   ByteCodeGenerationVisitor(CompileContext& context);

protected:
 // overloads
   virtual bool performStep(ASTNode& node);

private:
   CompileContext& mContext;
};

#endif // BYTECODE_GENERATION_VISITOR_H
