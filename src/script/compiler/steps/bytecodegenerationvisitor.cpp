
#include "bytecodegenerationvisitor.h"

#include "script/ast/ast.h"

ByteCodeGenerationVisitor::ByteCodeGenerationVisitor(CompileContext& context):
   CompileStep(),
   mContext(context)
{
}

// - Overloads

bool ByteCodeGenerationVisitor::performStep(ASTNode& node)
{
   node.accept(*this);

   return true;
}
