
#include "codegeneratorvisitor.h"

#include "core/defines.h"
#include "core/smartptr/scopedvalue.h"
#include "core/string/stringinterface.h"

#include "script/ast/ast.h"
#include "script/vm/virtualarray.h"
#include "script/vm/virtualclass.h"
#include "script/vm/virtualobject.h"
#include "script/vm/virtualinstruction.h"
#include "script/vm/virtualinstructiontable.h"
#include "script/vm/virtualclasstable.h"
#include "script/vm/virtualmachine.h"
#include "script/vm/virtualfunctiontable.h"
#include "script/vm/virtualfunctiontableentry.h"
#include "script/vm/virtuallookuptable.h"
#include "script/vm/virtualfunction.h"
#include "script/vm/virtualstring.h"

#include "script/common/literal.h"
#include "script/common/variant.h"
#include "script/common/classregistration.h"
#include "script/common/functionregistration.h"
#include "script/scope/scope.h"
#include "script/scope/scopevariable.h"
#include "script/scope/scopedscope.h"
#include "script/compiler/compilecontext.h"
#include "script/compiler/compiledclass.h"
#include "script/compiler/compiledfunction.h"
#include "script/compiler/signature.h"

#include "patch/callinterfacepatch.h"

using namespace CIL;

CodeGeneratorVisitor::CodeGeneratorVisitor(CompileContext& context):
   CompileStep(),
   mContext(context),
   mpClass(NULL),
   mpFunction(NULL),
   mpAccess(NULL),
   mpExpression(NULL),
   mCurrentType(),
   mpVClass(NULL),
   mInstructions(),
   mScopeStack(),
   mLoopFlowStack(),
   mpLookupTable(NULL),
   mLabel(0),
   mLineNr(0),
   mLoadFlags(0),
   mExpr(0),
   mState(0),
   mSuperCall(false),
   mRightHandSide(false),
   mStore(false),
   mNeedPop(true)
{
}

// - Overloads

bool CodeGeneratorVisitor::performStep(ASTNode& node)
{
   mInstructions.clear();
   mLineNr = 0;

   ((const ASTNode&)node).accept(*this);

   mContext.setResult(mpVClass);

   mpVClass->setDefinition(const_cast<ASTClass*>(mpClass));
   mpVClass->setStaticCount(mpClass->getTotalStatics());
   mpVClass = NULL;

   return true;
}

// - Code generation

void CodeGeneratorVisitor::addPatch(CodePatch* ppatch)
{
   ppatch->setVirtualFunction(*mpVirFunction);
   ppatch->setOffset(mLineNr);

   mPatches.push_back(ppatch);
}

// - Visitor

void CodeGeneratorVisitor::visit(const ASTRoot& root)
{
   visitChildren(root);
}

void CodeGeneratorVisitor::visit(const ASTClass& ast)
{
   ScopedScope scope(mScopeStack);

   mpClass = &ast;

   mpVClass = new VirtualClass();
   mpVClass->setName(ast.getFullName());
   mpVClass->setBaseName(ast.hasBaseClass() ? ast.getBaseClass().getFullName() : "");
   mpVClass->setVariableCount(ast.getTotalVariables());

   int flags = VirtualClass::eNone;
   if ( !ast.getModifiers().isAbstract() )
      flags |= VirtualClass::eInstantiatable;
   if ( ast.getModifiers().isNative() )
      flags |= VirtualClass::eNative;
   mpVClass->setFlags((VirtualClass::Flags)flags);

   // base class should be set by the VM
   // we don't register the member variables, as they must be loaded/stored differently than locals

   handleStaticBlock(ast);
   handleFieldBlock(ast);
   handleClassObject(ast);

   // maybe we should also let the loader fill in the virtual function table...
   // so for now just compile the functions

   const FunctionTable& table = ast.getFunctionTable();
   for ( int index = 0; index < table.size(); index++ )
   {
      const ASTFunction& function = table[index];
      if ( ast.isLocal(function) && !function.getModifiers().isAbstract() )
      {
         visit(function);
      }
      else if ( !function.getModifiers().isPureNative() )
      {
         VirtualFunctionTableEntry* pentry = new VirtualFunctionTableEntry();
         pentry->mName = function.getName();
         pentry->mArguments = function.getArgumentCount();

         mpVClass->getVirtualFunctionTable().append(pentry);
      }
   }

   ast.setState(ASTClass::eCompiled);
}

void CodeGeneratorVisitor::visit(const ASTFunction& ast)
{
   if ( ast.getModifiers().isPureNative() )
   {
      // No implementation, these are called directly except constructors. Those need to
      // be called explicitly.
   }
   else
   {
      ScopedScope scope(mScopeStack);

      const ASTFunction* pinterfacefnc = mpClass->findInterfaceFunction(ast);

      VirtualFunctionTableEntry* pentry = new VirtualFunctionTableEntry();
      pentry->mName = ast.getName();
      pentry->mInstruction = mInstructions.empty() ? 0 : mInstructions.back().linenr + 1;
      pentry->mOriginalInstruction = pentry->mInstruction;
      pentry->mArguments = ast.getArgumentCount();
      pentry->mInterface = pinterfacefnc != NULL ? pinterfacefnc->getResourceIndex() : -1;

      mpVClass->getVirtualFunctionTable().append(pentry);

      mpFunction = &ast;

      /* JB: no longer: done in the actual compiler
      // reserve space on the stack for arguments & local variables
      int varcount = ast.getLocalVariableCount();
      if ( varcount >= 1 )
      {
         addInstruction(VirtualInstruction::eReserve, varcount);
      }
      */

      mCurrentType.clear();

      ast.getBody().accept(*this);

      if ( ast.getType().isVoid() )
      {
         mBuilder.emit(CIL_ret, 0);
      }
   }
}

void CodeGeneratorVisitor::visit(const ASTVariableInit& ast)
{
   visitChildren(ast);
}

void CodeGeneratorVisitor::visit(const ASTArrayInit& ast)
{
   // do nothing, implemented elsewhere
}

void CodeGeneratorVisitor::visit(const ASTBlock& ast)
{
   ScopedScope scope(mScopeStack);

   visitChildren(ast);
}

void CodeGeneratorVisitor::visit(const ASTExpressionStatement& ast)
{
   mExpr = 0;
   mNeedPop = false;

   ast.getExpression().accept(*this);

   if ( mNeedPop )
   {
      mBuilder.addPop(1);
      mNeedPop = false;
   }
}

void CodeGeneratorVisitor::visit(const ASTLocalVariable& ast)
{
   const ASTVariable& var = ast.getVariable();

   CIL::Type* ptype = TypeToCILType(var.getType());
   mpCFunction->addLocal(ptype);

   if ( var.hasInit() )
   {
      mCurrentType.clear();

      const ASTVariableInit& varinit = var.getInit();
      if ( varinit.hasArrayInit() )
      {
         const ASTArrayInit& arrayinit = varinit.getArrayInit();

         mBuilder.addPush(arrayinit.getCount());
         mBuilder.addNewArray(1);
         mBuilder.addStoreLocal(var.getResourceIndex());

         int count = arrayinit.getCount();
         for ( int index = 0; index < count; index++ )
         {
            const ASTVariableInit& vinit = arrayinit.getInit(index);

            vinit.getExpression().accept(*this);

            mBuilder.emit(CIL_ldloc, var.getResourceIndex());
            mBuilder.emit(CIL_push, index);
            mBuilder.emit(CIL_stelem, 1);
         }
      }
      else
      {
         varinit.getExpression().accept(*this);

         mBuilder.emit(CIL_stloc, var.getResourceIndex());
      }
   }

   mScopeStack.add(ScopeVariable::fromVariable(var));
}

void CodeGeneratorVisitor::visit(const ASTIf& ast)
{
   ast.getCondition().accept(*this);

   int labelFalse = mBuilder.allocateLabel();

   mBuilder.addJumpFalse(labelFalse);

   ast.getStatement().accept(*this);

   if ( ast.hasElseStatement() )
   {
      int labelFinish = mBuilder.allocateLabel();
      mBuilder.addJump(labelFinish);

      mBuilder.addLabel(labelFalse);

      ast.getElseStatement().accept(*this);

      mBuilder.addLabel(labelFinish);
   }
   else
   {
      mBuilder.addLabel(labelFalse);
   }
}

void CodeGeneratorVisitor::visit(const ASTFor& ast)
{
   ScopedScope scope(mScopeStack);

   int labelStart = mBuilder.allocateLabel();

   LoopFlow flow;
   flow.start = mBuilder.allocateLabel();
   flow.end  = mBuilder.allocateLabel();
   mLoopFlowStack.push(flow);

   if ( ast.hasInitializer() )
   {
      ast.getInitializer().accept(*this);
   }

   mBuilder.addLabel(labelStart);

   if ( ast.hasCondition() )
   {
      ast.getCondition().accept(*this);

      mBuilder.addJumpFalse(flow.end);
   }

   ast.getBody().accept(*this);

   mBuilder.addLabel(flow.start);

   // update part
   mExpr = 0;
   visitChildren(ast);

   mBuilder.addJump(labelStart);
   mBuilder.addLabel(flow.end);

   mLoopFlowStack.pop();
}

void CodeGeneratorVisitor::visit(const ASTForeach& ast)
{
   ScopedScope scope(mScopeStack);

   LoopFlow flow;
   flow.start = mBuilder.allocateLabel();
   flow.end   = mBuilder.allocateLabel();
   mLoopFlowStack.push(flow);

   const ASTVariable& var = ast.getVariable();
   const ASTVariableInit& varinit = var.getInit();

   ASTTypeList list;
   Signature signature;

   mCurrentType.clear();
   varinit.getExpression().accept(*this);

   if ( mCurrentType.isArray() )
   {
      // index = 0
      mBuilder.emit(CIL_ldc, 0);
      mBuilder.emit(CIL_stloc, var.getResourceIndex());

      const ASTClass& arrayclass = mContext.resolveClass("system.InternalArray");
      const ASTField* pfield = arrayclass.findField("length", ASTClass::eLocal);

      // check for the size ( index < array.length )
      mBuilder.addLabel(flow.start);
      mBuilder.addDup();
      mBuilder.addLoad(pfield->getVariable().getResourceIndex());
      mBuilder.addLoadLocal(ast.getResourceIndex());
      mBuilder.addCmpEq();
      mBuilder.addJumpTrue(flow.end);
      
      // get item & execute body ( var = array[index]; )
      mBuilder.addDup();
      mBuilder.addLoadLocal(ast.getResourceIndex());
      mBuilder.addLoadElem(1);
      mBuilder.addStoreLocal(var.getResourceIndex());

      ast.getBody().accept(*this);

      // increment iteration ( index++; )
      mBuilder.addLoadLocal(ast.getResourceIndex());
      mBuilder.addConst(1);
      mBuilder.addAdd();
      mBuilder.addStoreLocal(ast.getResourceIndex());
      mBuilder.addJump(flow.start);

      mExpr = 0;

      // and of loop (pop array from stack)
      mBuilder.addLabel(flow.end);
      mBuilder.addPop(1);
   }
   else
   {
      String name = mCurrentType.getObjectClass().getFullName() + ".iterator";

      mBuilder.addPush(1);
      mBuilder.addCall(name);
      mBuilder.addStoreLocal(ast.getResourceIndex());

      mBuilder.addLabel(flow.start);

      String hasnextname = "engine.collections.Iterator.hasNext";
      String nextname = "engine.collections.Iterator.next";

      // is there still an item
      mBuilder.addLoadLocal(ast.getResourceIndex());
      mBuilder.addPush(1);
      mBuilder.addCall(hasnextname);
      mBuilder.addJumpFalse(flow.end);

      // store the next item from the list
      mBuilder.addLoadLocal(ast.getResourceIndex());
      mBuilder.addPush(1);
      mBuilder.addCall(nextname);
      mBuilder.addStoreLocal(var.getResourceIndex());

      ast.getBody().accept(*this);

      mBuilder.addJump(flow.start);
      mBuilder.addLabel(flow.end);
   }
}

void CodeGeneratorVisitor::visit(const ASTWhile& ast)
{
   LoopFlow flow;
   flow.start = mBuilder.allocateLabel();
   flow.end   = mBuilder.allocateLabel();
   mLoopFlowStack.push(flow);

   mBuilder.addLabel(flow.start);

   ast.getCondition().accept(*this);

   mBuilder.addJumpFalse(flow.end);

   ast.getBody().accept(*this);

   mBuilder.addJump(flow.start);
   mBuilder.addLabel(flow.end);

   mLoopFlowStack.pop();
}

void CodeGeneratorVisitor::visit(const ASTDo& ast)
{
   LoopFlow flow;
   flow.start = mBuilder.allocateLabel();
   flow.end   = mBuilder.allocateLabel();
   mLoopFlowStack.push(flow);

   mBuilder.addLabel(flow.start);

   ast.getBody().accept(*this);
   ast.getCondition().accept(*this);

   mBuilder.addJumpTrue(flow.start);
   mBuilder.addLabel(flow.end);

   mLoopFlowStack.pop();
}

void CodeGeneratorVisitor::visit(const ASTSwitch& ast)
{
   if ( ast.canLookup() )
   {
      // create a lookup table for the values (filled by the cases)
      mpLookupTable = new VirtualLookupTable();
      int tableidx = mpVClass->addLookupTable(mpLookupTable);

      ast.getExpression().accept(*this);

      // lookup the value in the table and jump there
      // if not found -> jump to default or skip in case no default is present
      mBuilder.addPush(tableidx);
      mBuilder.addLookup(mpClass->getFullName());

      visitChildren(ast);

      mpLookupTable->setEnd(mLineNr);
   }
   else
   {
      LoopFlow flow;
      flow.start = -1;
      flow.end   = mBuilder.allocateLabel();
      mLoopFlowStack.push(flow);

      // build all checks & jump to matching case/default statement
      int count = ast.getTotalCount();
      std::vector<int> labels;
      for ( int index = 0; index < count; index++ )
      {
         const ASTCase& astcase = ast.getCase(index);

         int label = mBuilder.allocateLabel();
         labels.push_back(label);

         if ( astcase.isDefault() )
         {
            mBuilder.emit(CIL_jump, label);

            // we break here as the other tests will never be executed
            // though the code can still fall through
            break;
         }
         else
         {
            ast.getExpression().accept(*this);
            astcase.getValueExpression().accept(*this);

            mBuilder.emit(CIL_cmpeq);
            mBuilder.emit(CIL_jump_true, label);
         }
      }

      if ( !ast.hasDefault() )
      {
         // if no default and none matched -> jump out
         mBuilder.addJump(flow.end);
      }

      // generate code for all bodies, with the correct labels before them
      for ( int index = 0; index < count; index++ )
      {
         const ASTCase& astcase = ast.getCase(index);

         mBuilder.addLabel(labels[index]);

         astcase.getBody().accept(*this);
      }

      mBuilder.addLabel(flow.end);
      mLoopFlowStack.pop();
   }
}

void CodeGeneratorVisitor::visit(const ASTCase& ast)
{
   ASSERT(ast.hasValue());
   ASSERT_PTR(mpLookupTable);

   if ( ast.isCase() )
   {
      mpLookupTable->add(ast.getValue(), mLineNr);
   }
   else
   {
      mpLookupTable->setDefault(mLineNr);
   }

   ast.getBody().accept(*this);
}

void CodeGeneratorVisitor::visit(const ASTReturn& ast)
{
   if ( ast.hasExpression() )
   {
      ast.getExpression().accept(*this);
   }

   mBuilder.addRet(ast.hasExpression() ? 1 : 0);
}

void CodeGeneratorVisitor::visit(const ASTTry& ast)
{
   int labelEnd   = mBuilder.allocateLabel();
   int labelCatch = mBuilder.allocateLabel();

   if ( ast.hasFinallyBlock() )
      mBuilder.addEnterGuardFinal(labelCatch);
   else
      mBuilder.addEnterGuard(labelCatch);

   ast.getBody().accept(*this);

   // if we get here, no exception happend
   mBuilder.addJump(labelEnd);


   // from here the code is executed when an exception did occure
   mBuilder.addLabel(labelCatch);
   mBuilder.addStoreLocal(ast.getResourceIndex());

   const ASTNodes& catches = ast.getCatches();
   for ( int index = 0; index < catches.size(); index++ )
   {
      const ASTCatch& c = static_cast<const ASTCatch&>(catches[index]);

      int labelNext = mBuilder.allocateLabel();

      const ASTVariable& var = c.getVariable().getVariable();

      mBuilder.addLoadLocal(ast.getResourceIndex());
      mBuilder.addInstanceOf(var.getType().getObjectClass().getFullName());
      mBuilder.addJumpFalse(labelNext);

      // make sure the exception variable can be resolved (is a variable after all)
      ScopedScope scope(mScopeStack);
      mScopeStack.add(ScopeVariable::fromVariable(var));

      c.getBody().accept(*this);

      mBuilder.addJump(labelEnd);
      mBuilder.addLabel(labelNext);
   }

   // the exception was not handled, so re-throw it
   mBuilder.addLoadLocal(ast.getResourceIndex());
   mBuilder.addThrow();

   // no exception occured or it was handled successfully
   mBuilder.addLabel(labelEnd);
   if ( ast.hasFinallyBlock() )
   {
      ast.getFinallyBlock().accept(*this);
   }

   mBuilder.addLeaveGuard();
}

void CodeGeneratorVisitor::visit(const ASTThrow& ast)
{
   ast.getExpression().accept(*this);

   mBuilder.addThrow();
}

void CodeGeneratorVisitor::visit(const ASTAssert& ast)
{
   ast.getCondition().accept(*this);

   int labelend = mBuilder.allocateLabel();
   String assertclass = "system.AssertionError";

   // if condition is false we throw an exception of type AssertionError,
   // else we jump to the end
   mBuilder.addJumpTrue(labelend);
   mBuilder.addNew(assertclass);
   mBuilder.addThrow();

   mBuilder.addLabel(labelend);
}

void CodeGeneratorVisitor::visit(const ASTLoopControl& ast)
{
   LoopFlow& info = mLoopFlowStack.top();

   // must become a stack of labels to support nested loops
   if ( ast.getKind() == ASTLoopControl::eBreak )
   {
      mBuilder.addJump(info.end);
   }
   else
   {
      mBuilder.addJump(info.start);
   }
}

void CodeGeneratorVisitor::visit(const ASTCompound& compound)
{
   compound.getExpression().accept(*this);
}

void CodeGeneratorVisitor::visit(const ASTExpression& ast)
{
   mCurrentType.clear();

   if ( ast.hasRight() )
   {
      mExpr++;

      ast.getRight().accept(*this);

      mStore = true;

      mCurrentType.clear();
      ast.getLeft().accept(*this);

      mStore = false;
      mNeedPop = false;

      ASSERT(mpAccess->getAccess() != ASTAccess::eInvalidAccess);
      bool local = mpAccess->getAccess() == ASTAccess::eLocal;

      if ( mpAccess->getAccess() == ASTAccess::eField )
         mBuilder.addLoadArg(0);

      switch ( ast.getKind() )
      {
         case ASTExpression::eAssign:
            handleAssignment(*mpAccess, local);
            break;

         case ASTExpression::ePlusAssign:
            break;
      }
   }
   else
   {
      mpExpression = &ast;
      mNeedPop = true;

      ast.getLeft().accept(*this);
   }

   mExpr++;
}

void CodeGeneratorVisitor::visit(const ASTConcatenate& concatenate)
{
   mExpr++;

   concatenate.getLeft().accept(*this);
   ASTType lefttype = mCurrentType;

   mCurrentType.clear();

   int labelResult = mBuilder.allocateLabel();
   int labelFinish = mBuilder.allocateLabel();

   switch ( concatenate.getMode() )
   {
      // arithmatic operators
      case ASTConcatenate::ePlus:
         {
            concatenate.getRight().accept(*this);

            mBuilder.addAdd();
         }
         break;

      case ASTConcatenate::eMinus:
         {
            concatenate.getRight().accept(*this);

            mBuilder.addSub();
         }
         break;

      case ASTConcatenate::eMul:
         {
            concatenate.getRight().accept(*this);

            mBuilder.addMul();
         }
         break;

      case ASTConcatenate::eDiv:
         {
            concatenate.getRight().accept(*this);

            mBuilder.addDiv();
         }
         break;

      case ASTConcatenate::eRem:
         {
            concatenate.getRight().accept(*this);

            mBuilder.addRem();
         }
         break;

      // bit operators

      case ASTConcatenate::eBitwiseOr:
         {
            concatenate.getRight().accept(*this);

            mBuilder.addOr();
         }
         break;

      case ASTConcatenate::eBitwiseXor:
         {
            concatenate.getRight().accept(*this);

            mBuilder.addXor();
         }
         break;

      case ASTConcatenate::eBitwiseAnd:
         {
            concatenate.getRight().accept(*this);

            mBuilder.addAnd();
         }
         break;

      case ASTConcatenate::eShiftLeft:
         {
            concatenate.getRight().accept(*this);

            mBuilder.addShiftLeft();
         }
         break;

      case ASTConcatenate::eShiftRight:
         {
            concatenate.getRight().accept(*this);

            mBuilder.addShiftRight();
         }
         break;

      // comparison

      case ASTConcatenate::eEquals:
         {
            SET_FLAG(mState, eStateNoNull);

            concatenate.getRight().accept(*this);

            CLEAR_FLAG(mState, eStateNoNull);

            switch ( mCurrentType.getKind() )
            {
               case ASTType::eNull:
                  mBuilder.addIsNull();
                  break;
               default:
                  mBuilder.addCmpEq();
            }
         }
         break;

      case ASTConcatenate::eUnequals:
         {
            SET_FLAG(mState, eStateNoNull);

            concatenate.getRight().accept(*this);

            CLEAR_FLAG(mState, eStateNoNull);

            switch ( mCurrentType.getKind() )
            {
               case ASTType::eNull:
                  mBuilder.addIsNull();
                  mBuilder.addNot();
                  break;
               default:
                  mBuilder.addCmpNe();
                  break;
            }
         }
         break;

      case ASTConcatenate::eSmallerEqual:
         {
            concatenate.getRight().accept(*this);

            mBuilder.addCmpLe();
         }
         break;

      case ASTConcatenate::eSmaller:
         {
            concatenate.getRight().accept(*this);

            mBuilder.addCmpLT();
         }
         break;

      case ASTConcatenate::eGreater:
         {
            concatenate.getRight().accept(*this);

            mBuilder.addCmpGt();
         }
         break;

      case ASTConcatenate::eGreaterEqual:
         {
            concatenate.getRight().accept(*this);

            mBuilder.addCmpGe();
         }
         break;

      // and/or

      case ASTConcatenate::eOr:
         {
            mBuilder.addJumpTrue(labelResult);                    // true

            concatenate.getRight().accept(*this);

            mBuilder.addJumpTrue(labelResult);                    // true
            mBuilder.addPush(0);                                  // push false
            mBuilder.addJump(labelFinish);                        // jump to finish
            mBuilder.addLabel(labelResult);
            mBuilder.addPush(1);                                  // push true
            mBuilder.addLabel(labelFinish);
         }
         break;

      case ASTConcatenate::eAnd:
         {
            mBuilder.addJumpFalse(labelResult);                   // false

            concatenate.getRight().accept(*this);

            mBuilder.addJumpFalse(labelResult);                   // false
            mBuilder.addPush(1);                                  // push true
            mBuilder.addJump(labelFinish);                        // jump to finish
            mBuilder.addLabel(labelResult);
            mBuilder.addPush(0);                                  // push false
            mBuilder.addLabel(labelFinish);
         }
         break;
   }
}

void CodeGeneratorVisitor::visit(const ASTUnary& ast)
{
   int index = 0;
   const ASTNodes& children = ast.getParts();
   for ( ; index < children.size() - 1; index++ )
   {
      const ASTNode& node = children[index];
      node.accept(*this);
   }

   if ( mStore )
   {
      mpAccess = dynamic_cast<const ASTAccess*>(&children[index]);
   }
   else if ( mRightHandSide )
   {
      // nothing is required here, we have our object
      if ( index == 0 )
      {
         // we need to push this as otherwise there is nothing
         mBuilder.addLoadArg(0);
      }
   }
   else
   {
      if ( ast.getPre() == ASTUnary::eIncrement )
         mLoadFlags = ePreIncr;
      else if ( ast.getPre() == ASTUnary::eDecrement )
         mLoadFlags = ePreDecr;

      if ( ast.getPost() == ASTUnary::eIncrement )
         mLoadFlags |= ePostIncr;
      else if ( ast.getPost() == ASTUnary::eDecrement )
         mLoadFlags |= ePostDecr;

      if ( mLoadFlags != 0 && mExpr > 0 )
      {
         SET_FLAG(mLoadFlags, eKeep); // determine if the result is still used!!
      }

      children[index].accept(*this);

      mLoadFlags = 0;

      switch ( ast.getPre() )
      {
         case ASTUnary::eNegate:
            mBuilder.addNeg();
            break;
         case ASTUnary::eNot:
            mBuilder.addNot();
            break;
      }
   }
}

void CodeGeneratorVisitor::visit(const ASTInstanceOf& ast)
{
   ast.getObject().accept(*this);

   String classname = ast.getInstanceType().getObjectClass().getFullName();
   mBuilder.addInstanceOf(classname);
}

void CodeGeneratorVisitor::visit(const ASTNew& ast)
{
   switch ( ast.getKind() )
   {
      case ASTNew::eObject:
         {
            String classname = ast.getType().getObjectClass().getFullName();
            mCurrentType = ast.getType();

            ast.getArguments().accept(*this);

            mBuilder.addNew(classname); // constructor index as argument
         }
         break;

      case ASTNew::eArray:
         {
            // reverse put sizes on stack
            reverseVisitChildren(ast);

            mBuilder.addNewArray(ast.getArrayDimension());
         }
         break;
   }
}

void CodeGeneratorVisitor::visit(const ASTSuper& ast)
{
   if ( ast.isCall() )
   {
      ASSERT(ast.hasConstructor());

      mBuilder.addLoadArg(0);

      // call to constructor of superclass
      visitChildren(ast);

      const ASTClass* pclass = &ast.getConstructor().getClass();
      if ( ast.isSuper() )
      {
         pclass = &pclass->getBaseClass();
      }

      String name = pclass->getFullName() + "." + pclass->getName();
      mBuilder.addCall(name);
      
      mNeedPop = false;
   }
   else if ( ast.isThis() )
   {
      mBuilder.addPushThis();
   }
   else
   {
      mSuperCall = true;
   }
}

void CodeGeneratorVisitor::visit(const ASTNative& ast)
{
   mBuilder.addPushThis();

   if ( ast.hasArguments() )
   {
      visitChildren(ast); // place arguments on the stack

      mBuilder.addPush(ast.getArguments().size() + 1);
   }
   else
   {
      ASSERT_PTR(mpFunction);
      int arguments = mpFunction->getArgumentCount();
      for ( int index = 1; index <= arguments; index++ ) // skip the 'this' argument
      {
         mBuilder.addLoadArg(index);
      }

      mBuilder.addPush(arguments + 1);
   }

   mBuilder.addCallNative(ast.getIndex());
}

void CodeGeneratorVisitor::visit(const ASTCast& ast)
{
   ast.getNode().accept(*this);

   ASTType from = mCurrentType;
   mCurrentType = ast.getType();

   // add cast instruction
   if ( mCurrentType.isObject() )
   {
   }
   else if ( mCurrentType.isArray() )
   {
   }
   else
   {
      switch ( mCurrentType.getKind() )
      {
         case ASTType::eInt:
            // should not yet happen
            mBuilder.addConvInt();
            break;
         case ASTType::eReal:
            mBuilder.addConvReal();
            break;
         case ASTType::eString:
            mBuilder.addConvString();
            break;
      }
   }
}

void CodeGeneratorVisitor::visit(const ASTAccess& ast)
{
   switch ( ast.getKind() )
   {
      case ASTAccess::eVariable:
         {
            const ASTVariable& var = ast.getVariable();
            bool isstatic = var.getModifiers().isStatic();

            switch ( ast.getAccess() )
            {
               case ASTAccess::eField:
                  if ( isstatic )
                  {
                     // use one of the two
                     // - mCurrentType : call static of other class
                     // - mpClass : access static field of this class
                     String name;
                     if ( mCurrentType.isValid() )
                        name = mCurrentType.getObjectClass().getFullName();
                     else
                        name = mpClass->getFullName();

                     mBuilder.emit(CIL_ldstr, name);
                  }
                  else
                  {
                     mBuilder.emit(CIL_ldarg, 0); // this
                  }

                  handleVariable(var, false);
                  break;

               case ASTAccess::eRefField:
                  if ( isstatic )
                  {
                     String name = mCurrentType.getObjectClass().getFullName();
                     mBuilder.emit(CIL_ldstr, name);
                  }
                  handleVariable(var, false);
                  break;

               case ASTAccess::eLocal:
                  handleVariable(var, true);
                  break;
            }

            const ASTType& type = var.getType();
            if ( mCurrentType.isValid() && type.isGeneric() )
            {
               const ASTTypeVariable& typevariable = type.getTypeVariable();
               if ( ast.getTypeArguments().size() > 0 )
               {
                  mCurrentType = ast.getTypeArguments()[typevariable.getIndex()];
               }
               else
               {
                  mCurrentType = mCurrentType.getTypeArguments()[typevariable.getIndex()];
               }
            }
            else
               mCurrentType = type;
         }
         break;

      case ASTAccess::eArray:
         {
            // the array object is now on top of the stack
            mExpr = 1;

            // add indices on stack
            reverseVisitChildren(ast);

            mBuilder.emit(CIL_ldelem, ast.getArguments().size());
         }
         break;

      case ASTAccess::eFunction:
         {
            const ASTFunction& function = ast.getFunction();

            if ( !mCurrentType.isValid() )
            {
               mBuilder.emit(CIL_ldarg, 0); // this
            }

            ASTType before = mCurrentType;

            visitChildren(ast);

            if ( function.getModifiers().isPureNative() )
            {
               const FunctionRegistration* preg = mContext.getClassRegistry().findCallback(function.getClass(), function);
               if ( preg == NULL )
               {
                  String fncname = function.getClass().getFullName() + "_" + function.getName() + "(" + mpFunction->getSignature().toString() + ")";
                  mContext.getLog().error("Can not find registered method for function " + fncname);
                  return;
               }

               mBuilder.emit(CIL_push, function.getArgumentCount());
               mBuilder.emit(CIL_call_native, preg->getIndex());
            }
            else if ( function.getModifiers().isStatic() ) // first check for static so native statics are possible as well
            {
               String name = before.isValid() ? before.getObjectClass().getFullName() : mpClass->getFullName();
               mBuilder.emit(CIL_call_static, name);
            }
            else 
            {
               mBuilder.emit(CIL_push, function.getArgumentCount());

               if ( before.isObject() && before.getObjectClass().getKind() == ASTClass::eInterface )
               {
                  addPatch(new CallInterfacePatch(before.getObjectClass(), function));

                  String name = before.getObjectClass().getFullName() + "." + function.getName();
                  mBuilder.emit(CIL_call, name);
               }
               else if ( mSuperCall )
               {
                  //addInstruction(VirtualInstruction::eCallSuper, function.getResourceIndex());
                  const ASTClass& baseclass = function.getClass().getBaseClass();
                  const ASTFunction& basefunction = baseclass.getFunctionTable()[function.getResourceIndex()];
                  String name = baseclass.getFullName() + "." + basefunction.getName();
                  mBuilder.emit(CIL_call, name);
               }
               else
               {
                  String name = function.getClass().getFullName() + "." + function.getName();
                  mBuilder.emit(CIL_call, name);
               }
            }

            if ( function.getType().isVoid() )
            {
               // nothing is pushed on the stack with a void function
               mNeedPop = false;
            }

            mSuperCall = false;

            const ASTType& type = function.getType();
            if ( type.isGeneric() )
            {
               const ASTTypeVariable& typevariable = type.getTypeVariable();
               mCurrentType = before.getTypeArguments()[typevariable.getIndex()];
            }
            else
               mCurrentType = function.getType();
         }
         break;

      case ASTAccess::eClass:
         {
            if ( ast.getAccess() == ASTAccess::eField )
            {
               mBuilder.emit(CIL_ldclass, 1);
            }
            else
            {
               mBuilder.emit(CIL_ldclass, mCurrentType.getObjectClass().getFullName());
            }

            mCurrentType.clear();
            mCurrentType.setKind(ASTType::eObject);
            mCurrentType.setObjectName("system.Class");
            mCurrentType.setObjectClass(mContext.resolveClass("system.Class"));
         }
         break;

      case ASTAccess::eStatic:
         {
            mCurrentType = ast.getStaticType();

            // the function/variable access pushes it's own label.
         }
         break;
   }

   mpAccess = &ast;
}

void CodeGeneratorVisitor::visit(const ASTLiteral& ast)
{
   if ( ast.getType().isNull() )
   {
      if ( !IS_SET(mState, eStateNoNull) )
      {
         mBuilder.addPushNull();
      }
   }
   else
   {
      handleLiteral(ast.getLiteral());
   }

   mCurrentType = ast.getType();
}

// - Helpers

void CodeGeneratorVisitor::handleAssignment(const ASTAccess& access, bool local)
{
   ScopedValue<bool> needspop(&mNeedPop, false, mNeedPop);

   switch ( access.getKind() )
   {
      case ASTAccess::eField:
         {
            bool isstatic = access.getVariable().getModifiers().isStatic();

            // the literal of the class has already been stored on the stack (see ASTAccess::eStatic and unary)

            if ( local )
            {
               mBuilder.addStoreLocal(access.getVariable().getResourceIndex());
            }
            else if ( isstatic )
            {
               mBuilder.addStoreStatic(access.getVariable().getResourceIndex());
            }
            else
            {
               mBuilder.addStore(access.getVariable().getResourceIndex());
            }
         }
         break;

      case ASTAccess::eArray:
         {
            // add indices on stack
            reverseVisitChildren(access);

            mBuilder.addStoreElem(access.getArguments().size());
         }
         break;

      case ASTAccess::eFunction:
         {
            // we should never get here
            UNREACHABLE("");
         }
         break;
   }
}

void CodeGeneratorVisitor::handleVariable(const ASTVariable& variable, bool local)
{
   bool isstatic = variable.getModifiers().isStatic();

   Opcode store = local ? CIL_stloc : (isstatic ? CIL_ststatic : CIL_store);
   Opcode load  = local ? CIL_ldloc : (isstatic ? CIL_ldstatic : CIL_load);

   if ( mLoadFlags > 0 )
   {
      // if not local, the object is already on the stack
      mBuilder.emit(load, variable.getResourceIndex());

      if ( variable.getType().isInt() )
         mBuilder.emit(CIL_ldc, 1);
      else
         mBuilder.emit(CIL_ldreal, 1.0);

      mBuilder.emit(IS_SET(mLoadFlags, ePreIncr) ? CIL_add : CIL_sub);
      mBuilder.emit(CIL_dup);

      int flags = mLoadFlags;
      mLoadFlags = 0;

      if ( !local )
      {
         // the unary will push this when required, after the visit an object is on top of the stack
         mRightHandSide = true;
         visit(*mpExpression);
         mRightHandSide = false;
      }

      mBuilder.emit(store, variable.getResourceIndex());

      if ( flags > eKeep )
      {
         // revert if it should be a post operator
         if ( variable.getType().isInt() )
            mBuilder.emit(CIL_ldc, 1);
         else
            mBuilder.emit(CIL_ldreal, 1.0);

         mBuilder.emit(IS_SET(flags, ePostIncr) ? CIL_add : CIL_sub);
      }
   }
   else
   {
      mBuilder.emit(load, variable.getResourceIndex());
   }
}

void CodeGeneratorVisitor::handleStaticBlock(const ASTClass& ast)
{
   VirtualFunctionTableEntry* pentry = new VirtualFunctionTableEntry();
   mpVClass->getVirtualFunctionTable().append(pentry);
   pentry->mName = "static_init";
   pentry->mInstruction = mInstructions.empty() ? 0 : mInstructions.back().linenr + 1;
   pentry->mOriginalInstruction = pentry->mInstruction;
   pentry->mArguments = 0;

   const ASTClass::Fields& fields = ast.getStatics();
   for ( std::size_t index = 0; index < fields.size(); index++ )
   {
      const ASTField* pfield = fields[index];
      const ASTVariable& variable = pfield->getVariable();

      if ( variable.hasInit() )
      {
         mCurrentType.clear();

         const ASTVariableInit& varinit = variable.getInit();
         if ( varinit.hasExpression() )
         {
            varinit.getExpression().accept(*this);

            mBuilder.emit(CIL_ldstr, ast.getFullName());
            mBuilder.emit(CIL_ststatic, variable.getResourceIndex());
         }
         else // array initializer
         {
            const ASTArrayInit& arrayinit = varinit.getArrayInit();

            // var = new array[count]
            mBuilder.emit(CIL_push, arrayinit.getCount());
            mBuilder.emit(CIL_newarray, 1);
            mBuilder.emit(CIL_ldstr, ast.getFullName());
            mBuilder.emit(CIL_ststatic, variable.getResourceIndex());

            int count = arrayinit.getCount();
            for ( int index = 0; index < count; index++ )
            {
               const ASTVariableInit& vinit = arrayinit.getInit(index);

               vinit.getExpression().accept(*this);

               // var[index] = expr
               mBuilder.emit(CIL_ldstr, ast.getFullName());
               mBuilder.emit(CIL_ldstatic, variable.getResourceIndex());
               mBuilder.emit(CIL_push, index);
               mBuilder.emit(CIL_stelem, 1);
            }
         }
      }
      else
      {
         const ASTType& type = variable.getType();
         switch ( type.getKind() )
         {
            case ASTType::eBoolean:
            case ASTType::eInt:
               mBuilder.emit(CIL_ldc, 0);
               break;
            case ASTType::eReal:
               mBuilder.emit(CIL_ldreal, 0.0);
               break;
            case ASTType::eString:
               mBuilder.emit(CIL_ldstr, "");
               break;
         }

         // class.var = init
         mBuilder.emit(CIL_push, ast.getFullName());
         mBuilder.emit(CIL_ststatic, variable.getResourceIndex());
      }
   }
   mBuilder.addRet(0);
}

void CodeGeneratorVisitor::handleFieldBlock(const ASTClass& ast)
{
   VirtualFunctionTableEntry* pentry = new VirtualFunctionTableEntry();
   mpVClass->getVirtualFunctionTable().append(pentry);
   pentry->mName = "var_init";
   pentry->mInstruction = mInstructions.empty() ? 0 : mInstructions.back().linenr + 1;
   pentry->mOriginalInstruction = pentry->mInstruction;
   pentry->mArguments = 1;

   if ( ast.hasBaseClass() )
   {
      String name = ast.getBaseClass().getFullName() + ".field_init";
      mBuilder.emit(CIL_ldarg, 0);
      mBuilder.emit(CIL_call, name);
   }

   const ASTClass::Fields& fields = ast.getFields();
   for ( std::size_t index = 0; index < fields.size(); index++ )
   {
      const ASTField* pfield = fields[index];
      const ASTVariable& variable = pfield->getVariable();

      if ( variable.hasInit() )
      {
         mCurrentType.clear();

         const ASTVariableInit& varinit = variable.getInit();
         if ( varinit.hasExpression() )
         {
            varinit.getExpression().accept(*this);

            mBuilder.emit(CIL_ldarg, 0);
            mBuilder.emit(CIL_store, variable.getResourceIndex());
         }
         else // array initialization (currently only for one dimensional arrays)
         {
            const ASTArrayInit& arrayinit = varinit.getArrayInit();

            mBuilder.emit(CIL_push, arrayinit.getCount());
            mBuilder.emit(CIL_newarray, 1);
            mBuilder.emit(CIL_ldarg, 0);
            mBuilder.emit(CIL_store, variable.getResourceIndex());

            int count = arrayinit.getCount();
            for ( int index = 0; index < count; index++ )
            {
               const ASTVariableInit& vinit = arrayinit.getInit(index);

               vinit.getExpression().accept(*this);

               mBuilder.emit(CIL_ldarg, 0);
               mBuilder.emit(CIL_load, variable.getResourceIndex());
               mBuilder.emit(CIL_push, index);
               mBuilder.emit(CIL_stelem, 1);
            }
         }
      }
      else
      {
         const ASTType& type = variable.getType();
         switch ( type.getKind() )
         {
            case ASTType::eBoolean:
            case ASTType::eInt:
               mBuilder.emit(CIL_ldc, 0);
               break;
            case ASTType::eReal:
               mBuilder.emit(CIL_ldreal, 0.0);
               break;
            case ASTType::eString:
               mBuilder.emit(CIL_ldstr, "");
               break;
         }

         mBuilder.emit(CIL_ldarg, 0);
         mBuilder.emit(CIL_store, variable.getResourceIndex());
      }
   }
   mBuilder.addRet(0);
}

void CodeGeneratorVisitor::handleClassObject(const ASTClass& ast)
{
   const FunctionTable& table = ast.getFunctionTable();
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

   mpVClass->setClassObject(classobject);
}

void CodeGeneratorVisitor::handleLiteral(const Literal& literal)
{
   const Variant& value = literal.getValue();
   if ( value.isInt() )
   {
      mBuilder.emit(CIL_ldc, value.asInt());
   }
   else if ( value.isReal() )
   {
      mBuilder.emit(CIL_ldreal, value.asReal());
   }
   else if ( value.isBool() )
   {
      mBuilder.emit(CIL_ldc, value.asBool() ? 1 : 0);
   }
   else if ( value.isString() )
   {
      mBuilder.emit(CIL_ldstr, value.asString().getString());
   }
   else if ( value.isChar() )
   {
      mBuilder.emit(CIL_ldchar, value.asChar());
   }
   else
   {
      // should not get here
   }
}

CIL::Type* CodeGeneratorVisitor::TypeToCILType(const ASTType& type)
{
   CIL::Type* ptype = new CIL::Type;

   switch ( type.getKind() )
   {
      case ASTType::eBoolean:
         ptype->type = CIL::eBool;
         break;
      case ASTType::eInt:
         ptype->type = CIL::eInt;
         break;
      case ASTType::eReal:
         ptype->type = CIL::eReal;
         break;
      case ASTType::eChar:
         ptype->type = CIL::eChar;
         break;
      case ASTType::eString:
         ptype->type = CIL::eString;
         break;
      case ASTType::eArray:
         ptype->type = CIL::eArray;
         ptype->size = type.getArrayDimension();
         ptype->elem_type = TypeToCILType(type.getArrayType());
         break;
      case ASTType::eObject:
         ptype->type = CIL::eObject;
         ptype->name = new String(type.getObjectClass().getFullName());
         break;
      case ASTType::eVoid:
         ptype->type = CIL::eVoid;
         break;
      default:
         UNREACHABLE("type is not yet converted.");
   }

   return ptype;
}

// - Patching interface



void CodeGeneratorVisitor::applyPatches()
{
   for ( unsigned int index = 0; index < mPatches.size(); ++index )
   {
      CodePatch* ppatch = mPatches[index];
      Inst& inst = mInstructions[ppatch->getOffset()];
      
      switch ( ppatch->getKind() )
      {
         case CodePatch::eCallInterface:
            {
               CallInterfacePatch* pcall = static_cast<CallInterfacePatch*>(ppatch);
               ASTClass& klass = pcall->getClass();

               printf("Patching interface call\n");


               //pcall->getFunction().get
               inst.arg = 1;
            }
            break;
      }
   }
}
