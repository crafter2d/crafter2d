
#include "bytecodegenerationvisitor.h"

#include "script/ast/ast.h"
#include "script/compiler/compilecontext.h"
#include "script/bytecode/irgenerator.h"
#include "script/bytecode/program.h"
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

   VirtualClass* pbaseclass = NULL;
   if ( ast.hasBaseClass() )
   {
      pbaseclass = &mContext.resolveVirtualClass(ast.getBaseClass().getFullName());
   }

   mpVirClass = new VirtualClass();
   mpVirClass->setName(ast.getFullName());
   mpVirClass->setBaseClass(*pbaseclass);
   mpVirClass->setVariableCount(ast.getTotalVariables());

   int flags = VirtualClass::eNone;
   if ( !ast.getModifiers().isAbstract() )
      flags |= VirtualClass::eInstantiatable;
   if ( ast.getModifiers().isNative() )
      flags |= VirtualClass::eNative;
   mpVirClass->setFlags((VirtualClass::Flags)flags);

   const ASTFunctionTable& functions = ast.getFunctionTable();
   for ( int index = 0; index < functions.size(); ++index )
   {
      const ASTFunction& function = functions[index];
      if ( ast.isLocal(function) )
      {
         if ( function.getModifiers().isAbstract() || function.getModifiers().isPureNative() )
         {
            // abstract functions are added here
            VirtualFunctionTableEntry* pentry = new VirtualFunctionTableEntry();
            pentry->mName = function.getName();
            pentry->mArguments = function.getArgumentCount();
            mpVirClass->getVirtualFunctionTable().append(pentry);
         }
         else
         {
            visit(function);  
         }
      }
      else
      {
         ASSERT_PTR(pbaseclass);
         // call to method of a base class
         VirtualFunctionTableEntry* pentry = pbaseclass->getVirtualFunctionTable()[index].clone();
         mpVirClass->getVirtualFunctionTable().append(pentry);
      }
   }

   mpVirClass->setByteCode(mpCode);

   mContext.addVirtualClass(mpVirClass);

   delete[] mpCode;
   mpCode = NULL;
}

void ByteCodeGenerationVisitor::visit(const ASTFunction& ast)
{
   ByteCode::Program program;
   ByteCode::IRGenerator& generator = mContext.getByteCodeGenerator();
   char* pcode = generator.generate(mContext, program, ast);

   VirtualFunctionTableEntry* pentry = new VirtualFunctionTableEntry();
   pentry->mName = ast.getPrototype();
   pentry->mInstruction = 0; // insert offset in byte code
   pentry->mOriginalInstruction = pentry->mInstruction;
   pentry->mArguments = ast.getArgumentCount();

   mpVirClass->getVirtualFunctionTable().append(pentry);
}
