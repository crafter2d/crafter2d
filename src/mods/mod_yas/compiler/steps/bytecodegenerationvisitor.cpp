
#include "bytecodegenerationvisitor.h"

#include "script/ast/ast.h"
#include "script/compiler/compilecontext.h"
#include "script/bytecode/irgenerator.h"
#include "script/bytecode/program.h"
#include "script/cil/guard.h"
#include "script/vm/virtualarray.h"
#include "script/vm/virtualclass.h"
#include "script/vm/virtualobject.h"
#include "script/vm/virtualfunctiontableentry.h"
#include "script/vm/virtualguard.h"

ByteCodeGenerationVisitor::ByteCodeGenerationVisitor(CompileContext& context):
   CompileStep(),
   mContext(context)
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
   VirtualClass* pbaseclass = NULL;
   if ( ast.hasBaseClass() )
   {
      pbaseclass = &mContext.resolveVirtualClass(ast.getBaseClass().getFullName());
   }

   mpVirClass = new VirtualClass();
   mpVirClass->setDefinition(ast);
   mpVirClass->setName(ast.getFullName());
   mpVirClass->setBaseClass(*pbaseclass);
   mpVirClass->setVariableCount(ast.getTotalVariables());
   mpVirClass->setStaticCount(ast.getStatics().size());

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
            pentry->returns = !function.getType().isVoid();
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

   createClassObject(ast);
   createInterfaceLookupTable(ast);

   mContext.addVirtualClass(mpVirClass);
}

void ByteCodeGenerationVisitor::visit(const ASTFunction& ast)
{
   ByteCode::IRGenerator& generator = mContext.getByteCodeGenerator();
   VirtualFunctionTableEntry* pentry =  generator.generate(mContext, ast);
   if ( pentry == NULL )
   {
      // complain! mehh
   }

   pentry->mName = ast.getName();
   pentry->mArguments = ast.getArgumentCount();
   pentry->mLocals = ast.getLocals().size();
   pentry->returns = !ast.getType().isVoid();

   mpVirClass->getVirtualFunctionTable().append(pentry);
}

// - Operations

void ByteCodeGenerationVisitor::createClassObject(const ASTClass& ast)
{
   const ASTFunctionTable& table = ast.getFunctionTable();
   VirtualArray* pfuncarray = new VirtualArray();
   pfuncarray->addLevel(table.size());

   for ( int index = 0; index < table.size(); index++ )
   {
      const ASTFunction& function = table[index];

      VirtualArray* pannoarray = new VirtualArray();
      if ( function.hasAnnotations() )
      {
         const ASTAnnotations& annotations = function.getAnnotations();
         pannoarray->addLevel(annotations.size());
         for ( int a = 0; a < annotations.size(); a++ )
         {
            const ASTAnnotation& annotation = annotations[a];

            VirtualString& vname = mContext.getStringCache().lookup(annotation.mName);
            (*pannoarray)[a] = Variant(vname);
         }
      }
      else
      {
         pannoarray->addLevel(0);
      }

      VirtualObject* funcobject = new VirtualObject();
      funcobject->initialize(2);
      funcobject->setMember(0, Variant(mContext.getStringCache().lookup(function.getName())));
      funcobject->setMember(1, Variant(*pannoarray));

      (*pfuncarray)[index] = Variant(*funcobject); // <-- hack!
   }

   VirtualObject* classobject = new VirtualObject();
   classobject->initialize(2);
   classobject->setMember(0, Variant(mContext.getStringCache().lookup(ast.getFullName())));
   classobject->setMember(1, Variant(*pfuncarray));

   mpVirClass->setClassObject(classobject);
}

void ByteCodeGenerationVisitor::createInterfaceLookupTable(const ASTClass& klass)
{
   ASTTypeList interfaces;
   klass.collectInterfaces(interfaces);

   // determine the highest number of used function
   int max = 0;
   for ( int index = 0; index < interfaces.size(); ++index )
   {
      const ASTClass& iface = interfaces[index].getObjectClass();
      const ASTFunctionTable& table = iface.getFunctionTable();
      ASSERT(table.size() > 0);

      const ASTFunction& function = table[table.size() - 1];
      if ( function.getResourceIndex() > max )
      {
         max = function.getResourceIndex();
      }
   }

   const ASTFunctionTable& klasstable = klass.getFunctionTable();

   // create the lookup table (each interface function is at the same position in the table
   // and links to the actual function number in the virtual table.
   int* ptable = new int[max + 1];
   for ( int index = 0; index < interfaces.size(); ++index )
   {
      const ASTClass& iface = interfaces[index].getObjectClass();
      const ASTFunctionTable& table = iface.getFunctionTable();
      
      for ( int entry = 0; entry < table.size(); ++entry )
      {
         const ASTFunction& func = table[entry];
         const ASTFunction* plookupfunc = klass.findExactMatch(func.getName(), func.getSignature());

         ASSERT_PTR(plookupfunc);
         ASSERT(func.getResourceIndex() <= max);

         ptable[func.getResourceIndex()] = plookupfunc->getResourceIndex();
      }
   }

   mpVirClass->setInterfaceLookupTable(ptable);
}
