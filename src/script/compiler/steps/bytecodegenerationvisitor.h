
#ifndef BYTECODE_GENERATION_VISITOR_H
#define BYTECODE_GENERATION_VISITOR_H

#include "script/compiler/compilestep.h"

class CompileContext;
class VirtualClass;
class VirtualFunctionTableEntry;

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
 // operations
   void handleClassObject(const ASTClass& ast);

   CompileContext& mContext;
   VirtualClass*   mpVirClass;
};

#endif // BYTECODE_GENERATION_VISITOR_H
