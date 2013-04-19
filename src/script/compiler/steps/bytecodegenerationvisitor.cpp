
#include "bytecodegenerationvisitor.h"

#include "script/ast/ast.h"
#include "script/compiler/compilecontext.h"
#include "script/vm/codegen/irgenerator.h"
#include "script/vm/virtualclass.h"
#include "script/vm/virtualfunctiontableentry.h"

ByteCodeGenerationVisitor::ByteCodeGenerationVisitor(CompileContext& context):
   CompileStep(),
   mContext(context),
   mpCode(NULL),
   mCodeSize(0)
{
}

// - Overloads

bool ByteCodeGenerationVisitor::performStep(ASTNode& node)
{
   ((const ASTNode&)node).accept(*this);

   return true;
}

void ByteCodeGenerationVisitor::visit(const ASTRoot& root)
{
   visitChildren(root);
}

void ByteCodeGenerationVisitor::visit(const ASTClass& ast)
{
   mCodeSize = 64;
   mpCode = new char[mCodeSize];

   mpVirClass = new VirtualClass();
   mpVirClass->setName(ast.getName());
   mpVirClass->setVariableCount(ast.getTotalVariables());

   int flags = VirtualClass::eNone;
   if ( !ast.getModifiers().isAbstract() )
      flags |= VirtualClass::eInstantiatable;
   if ( ast.getModifiers().isNative() )
      flags |= VirtualClass::eNative;
   mpVirClass->setFlags((VirtualClass::Flags)flags);

   if ( ast.hasBaseClass() )
   {
      VirtualClass& baseclass = mContext.resolveVirtualClass(ast.getBaseClass().getFullName());
      mpVirClass->setBaseClass(baseclass);
   }

   const ASTFunctionTable& functions = ast.getFunctionTable();

   mpVirClass->setByteCode(mpCode);

   delete[] mpCode;
   mpCode = NULL;
}

void ByteCodeGenerationVisitor::visit(const ASTFunction& ast)
{
   if ( ast.getModifiers().isPureNative() )
   {
      // nothing to generate
   }
   else
   {
      CodeGen::IRGenerator& generator = mContext.getByteCodeGenerator();
      char* pcode = generator.generate(mContext, ast);

      VirtualFunctionTableEntry* pentry = new VirtualFunctionTableEntry();
      pentry->mName = ast.getPrototype();
      pentry->mInstruction = 0; // insert offset in byte code
      pentry->mOriginalInstruction = pentry->mInstruction;
      pentry->mArguments = ast.getArgumentCount();

      mpVirClass->getVirtualFunctionTable().append(pentry);
   }
}
