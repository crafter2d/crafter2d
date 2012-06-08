
#include "codegeneratorvisitor.h"

#include "core/defines.h"

#include "script/ast/ast.h"
#include "script/vm/virtualarrayobject.h"
#include "script/vm/virtualclass.h"
#include "script/vm/virtualobject.h"
#include "script/vm/virtualinstruction.h"
#include "script/vm/virtualinstructiontable.h"
#include "script/vm/virtualclasstable.h"
#include "script/vm/virtualmachine.h"
#include "script/vm/virtualfunctiontable.h"
#include "script/vm/virtualfunctiontableentry.h"
#include "script/vm/virtualarrayreference.h"
#include "script/vm/virtuallookuptable.h"

#include "script/common/literal.h"
#include "script/common/variant.h"
#include "script/scope/scope.h"
#include "script/scope/scopevariable.h"
#include "script/scope/scopedscope.h"
#include "script/compiler/compilecontext.h"
#include "script/compiler/signature.h"

const int labelID = 0xF000;

CodeGeneratorVisitor::CodeGeneratorVisitor(CompileContext& context):
   CompileStep(),
   mContext(context),
   mpClass(NULL),
   mpFunction(NULL),
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
   mStore(false)
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

void CodeGeneratorVisitor::fillInstructionList()
{
   convertLabels();

   VirtualInstructionTable& instructions = mpVClass->getInstructions();
   for ( std::size_t index = 0; index < mInstructions.size(); index++ )
   {
      Inst& inst = mInstructions[index];
      instructions.add((VirtualInstruction::Instruction)inst.instruction, inst.arg);
   }
}

int CodeGeneratorVisitor::findLabel(int label) const
{
   std::size_t index = 0;
   for ( ; index < mInstructions.size(); index++ )
   {
      const Inst& inst = mInstructions[index];
      if ( inst.instruction == labelID && inst.arg == label )
      {
         break;
      }
   }

   return index;
}

void CodeGeneratorVisitor::convertLabels()
{
   if ( mInstructions.size() > 0 )
   {
      int firstline = mInstructions[0].linenr;

      for ( std::size_t index = 0; index < mInstructions.size(); index++ )
      {
         Inst& inst = mInstructions[index];

         VirtualInstruction::Instruction instruction = (VirtualInstruction::Instruction)inst.instruction;
         if ( instruction == VirtualInstruction::eJump
           || instruction == VirtualInstruction::eJumpTrue
           || instruction == VirtualInstruction::eJumpFalse
           || instruction == VirtualInstruction::eEnterGuard
           || instruction == VirtualInstruction::eEnterGuardF )
         {
            int labelindex = findLabel(inst.arg);
            ASSERT(labelindex != mInstructions.size());

            const Inst& labelinst = mInstructions[labelindex];
            inst.arg = labelinst.linenr - firstline;
         }
      }

      removeLabels();
   }
}

void CodeGeneratorVisitor::removeLabels()
{
   for ( int index = mInstructions.size() - 1; index >= 0; index-- )
   {
      Inst& inst = mInstructions[index];
      if ( inst.instruction == labelID )
      {
         mInstructions.erase(mInstructions.begin() + index);
      }
   }
}

void CodeGeneratorVisitor::addInstruction(int instruction, int argument)
{
   Inst inst;
   inst.instruction = instruction;
   inst.arg         = argument;
   inst.linenr      = mLineNr++;

   mInstructions.push_back(inst);
}

void CodeGeneratorVisitor::addLabel(int id)
{
   Inst inst;
   inst.instruction = labelID;
   inst.arg         = id;
   inst.linenr      = mLineNr;

   mInstructions.push_back(inst);
}

int CodeGeneratorVisitor::allocateLabel()
{
   return mLabel++;
}

int CodeGeneratorVisitor::allocateLiteral(const std::string& value)
{
   Variant variant(value);

   int index = mContext.getLiteralTable().indexOf(variant);
   if ( index == mContext.getLiteralTable().size() )
   {
      Literal* pliteral = new Literal(variant);
      index = mContext.getLiteralTable().insert(pliteral);
   }

   return index;
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
      else
      {
         VirtualFunctionTableEntry* pentry = new VirtualFunctionTableEntry();
         pentry->mName = function.getName();
         pentry->mArguments = function.getArgumentCount();

         mpVClass->getVirtualFunctionTable().append(pentry);
      }
   }

   fillInstructionList();

   ast.setState(ASTClass::eCompiled);
}

void CodeGeneratorVisitor::visit(const ASTFunction& ast)
{
   if ( ast.getModifiers().isNative() )
   {
      // here a native method should be invoked

      VirtualFunctionTableEntry* pentry = new VirtualFunctionTableEntry();
      pentry->mName = ast.getName();
      pentry->mInstruction = mInstructions.empty() ? 0 : mInstructions.back().linenr + 1;
      pentry->mOriginalInstruction = pentry->mInstruction;
      pentry->mArguments = ast.getArgumentCount();

      mpVClass->getVirtualFunctionTable().append(pentry);

      if ( ast.isConstructor() )
      {
         // call the init method -> set the native object
         std::string fncname = mpClass->getName() + "_init";
         int resource = allocateLiteral(fncname);

         addInstruction(VirtualInstruction::ePush, ast.getArgumentCount());
         addInstruction(VirtualInstruction::eNewNative, resource);

         if ( ast.hasBody() )
         {
            ast.getBody().accept(*this);
         }

         addInstruction(VirtualInstruction::eRet, 0);
      }
      else
      {
         std::string fncname = ast.getClass().getName() + "_" + ast.getName();
         int resource = allocateLiteral(fncname);

         // the arguments of this function are re-used by the native function,
         // so no need to push them on the stack again

         addInstruction(VirtualInstruction::ePush, ast.getArgumentCount()); // includes this
         addInstruction(VirtualInstruction::eCallNative, resource);
         addInstruction(VirtualInstruction::eRet, ast.getType().isVoid() ? 0 : 1);
      }
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

      // reserve space on the stack for arguments & local variables
      int varcount = ast.getLocalVariableCount();
      if ( varcount > 1 )
      {
         addInstruction(VirtualInstruction::eReserve, varcount);
      }

      mCurrentType.clear();

      ast.getBody().accept(*this);

      if ( ast.getType().isVoid() )
      {
         addInstruction(VirtualInstruction::eRet, 0);
      }
   }
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
      addInstruction(VirtualInstruction::ePop, 1);
      mNeedPop = false;
   }
}

void CodeGeneratorVisitor::visit(const ASTLocalVariable& ast)
{
   const ASTVariable& var = ast.getVariable();

   if ( var.hasInit() )
   {
      mCurrentType.clear();

      const ASTVariableInit& varinit = var.getInit();
      varinit.getExpression().accept(*this);

      addInstruction(VirtualInstruction::eStoreLocal, var.getResourceIndex());
   }

   mScopeStack.add(ScopeVariable::fromVariable(var));
}

void CodeGeneratorVisitor::visit(const ASTIf& ast)
{
   ast.getCondition().accept(*this);

   int labelFalse = allocateLabel();
   addInstruction(VirtualInstruction::eJumpFalse, labelFalse);

   ast.getStatement().accept(*this);

   if ( ast.hasElseStatement() )
   {
      int labelFinish = allocateLabel();
      addInstruction(VirtualInstruction::eJump, labelFinish);

      addLabel(labelFalse);

      ast.getElseStatement().accept(*this);

      addLabel(labelFinish);
   }
   else
   {
      addLabel(labelFalse);
   }
}

void CodeGeneratorVisitor::visit(const ASTFor& ast)
{
   ScopedScope scope(mScopeStack);

   int labelStart = allocateLabel();

   LoopFlow flow;
   flow.start = allocateLabel();
   flow.end  = allocateLabel();
   mLoopFlowStack.push(flow);

   if ( ast.hasInitializer() )
   {
      ast.getInitializer().accept(*this);
   }

   addLabel(labelStart);

   if ( ast.hasCondition() )
   {
      ast.getCondition().accept(*this);

      addInstruction(VirtualInstruction::eJumpFalse, flow.end);
   }

   ast.getBody().accept(*this);

   addLabel(flow.start);

   // update part
   mExpr = 0;
   visitChildren(ast);

   addInstruction(VirtualInstruction::eJump, labelStart);

   addLabel(flow.end);

   mLoopFlowStack.pop();
}

void CodeGeneratorVisitor::visit(const ASTForeach& ast)
{
   ScopedScope scope(mScopeStack);

   LoopFlow flow;
   flow.start = allocateLabel();
   flow.end  = allocateLabel();
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
      addInstruction(VirtualInstruction::eInt0);
      addInstruction(VirtualInstruction::eStoreLocal, ast.getResourceIndex());

      const ASTClass& arrayclass = mContext.resolveClass("system.InternalArray");
      const ASTField* pfield = arrayclass.findField("length", ASTClass::eLocal);

      // check for the size ( index < array.length )
      addLabel(flow.start);
      addInstruction(VirtualInstruction::eDup);
      addInstruction(VirtualInstruction::eLoad, pfield->getVariable().getResourceIndex());
      addInstruction(VirtualInstruction::eLoadLocal, ast.getResourceIndex());
      addInstruction(VirtualInstruction::eCmpEqInt);
      addInstruction(VirtualInstruction::eJumpTrue, flow.end);

      // get item & execute body ( var = array[index]; )
      addInstruction(VirtualInstruction::eDup);
      addInstruction(VirtualInstruction::eLoadLocal, ast.getResourceIndex());
      addInstruction(VirtualInstruction::eLoadArray, 1);
      addInstruction(VirtualInstruction::eStoreLocal, var.getResourceIndex());

      ast.getBody().accept(*this);

      // increment iteration ( index++; )
      addInstruction(VirtualInstruction::eLoadLocal, ast.getResourceIndex());
      addInstruction(VirtualInstruction::eInt1);
      addInstruction(VirtualInstruction::eAddInt);
      addInstruction(VirtualInstruction::eStoreLocal, ast.getResourceIndex());
      addInstruction(VirtualInstruction::eJump, flow.start);
      mExpr = 0;

      // and of loop (pop array from stack)
      addLabel(flow.end);
      addInstruction(VirtualInstruction::ePop, 1);
   }
   else
   {
      const ASTFunction* piterator = mCurrentType.getObjectClass().findBestMatch("iterator", signature, list);

      addInstruction(VirtualInstruction::ePush, 1);
      addInstruction(VirtualInstruction::eCall, piterator->getResourceIndex());
      addInstruction(VirtualInstruction::eStoreLocal, ast.getResourceIndex());

      addLabel(flow.start);

      const ASTClass& iteratorclass = mContext.resolveClass("engine.collections.Iterator");
      const ASTFunction* phasnext = iteratorclass.findBestMatch("hasNext", signature, list);
      const ASTFunction* pnext = iteratorclass.findBestMatch("next", signature, list);

      // is there still an item
      addInstruction(VirtualInstruction::eLoadLocal, ast.getResourceIndex());
      addInstruction(VirtualInstruction::ePush, 1);
      addInstruction(VirtualInstruction::eCall, phasnext->getResourceIndex());
      addInstruction(VirtualInstruction::eJumpFalse, flow.end);

      // store the next item from the list
      addInstruction(VirtualInstruction::eLoadLocal, ast.getResourceIndex());
      addInstruction(VirtualInstruction::ePush, 1);
      addInstruction(VirtualInstruction::eCall, pnext->getResourceIndex());
      addInstruction(VirtualInstruction::eStoreLocal, var.getResourceIndex());

      ast.getBody().accept(*this);

      addInstruction(VirtualInstruction::eJump, flow.start);
      addLabel(flow.end);
   }
}

void CodeGeneratorVisitor::visit(const ASTWhile& ast)
{
   LoopFlow flow;
   flow.start = allocateLabel();
   flow.end  = allocateLabel();
   mLoopFlowStack.push(flow);

   addLabel(flow.start);

   ast.getCondition().accept(*this);

   addInstruction(VirtualInstruction::eJumpFalse, flow.end);

   ast.getBody().accept(*this);

   addInstruction(VirtualInstruction::eJump, flow.start);

   addLabel(flow.end);

   mLoopFlowStack.pop();
}

void CodeGeneratorVisitor::visit(const ASTDo& ast)
{
   LoopFlow flow;
   flow.start = allocateLabel();
   flow.end  = allocateLabel();
   mLoopFlowStack.push(flow);

   addLabel(flow.start);

   ast.getBody().accept(*this);
   ast.getCondition().accept(*this);

   addInstruction(VirtualInstruction::eJumpTrue, flow.start);

   addLabel(flow.end);

   mLoopFlowStack.pop();
}

void CodeGeneratorVisitor::visit(const ASTSwitch& ast)
{
   LoopFlow flow;
   flow.start = -1;
   flow.end  = allocateLabel();
   mLoopFlowStack.push(flow);

   if ( ast.canLookup() )
   {
      // create a lookup table for the values (filled by the cases)
      mpLookupTable = new VirtualLookupTable();
      int tableidx = mpVClass->addLookupTable(mpLookupTable);

      ast.getExpression().accept(*this);

      // lookup the value in the table and jump there
      // if not found -> jump to default or skip in case no default is present
      addInstruction(VirtualInstruction::ePush, allocateLiteral(mpClass->getFullName()));
      addInstruction(VirtualInstruction::eLookup, tableidx);

      visitChildren(ast);

      mpLookupTable->setEnd(mLineNr);
   }
   else
   {
      // build all checks & jump to matching case/default statement
      int count = ast.getTotalCount();
      std::vector<int> labels;
      for ( int index = 0; index < count; index++ )
      {
         const ASTCase& astcase = ast.getCase(index);

         int label = allocateLabel();
         labels.push_back(label);

         if ( astcase.isDefault() )
         {
            addInstruction(VirtualInstruction::eJump, label);

            // we break here as the other tests will never be executed
            // though the code can still fall through
            break;
         }
         else
         {
            ast.getExpression().accept(*this);
            astcase.getValueExpression().accept(*this);

            switch ( astcase.getType().getKind() )
            {
               case ASTType::eInt:
                  addInstruction(VirtualInstruction::eCmpEqInt);
                  break;
               case ASTType::eReal:
                  addInstruction(VirtualInstruction::eCmpEqReal);
                  break;
               case ASTType::eString:
                  addInstruction(VirtualInstruction::eCmpEqStr);
                  break;
            }

            addInstruction(VirtualInstruction::eJumpTrue, label);
         }
      }

      if ( !ast.hasDefault() )
      {
         // if no default and none matched -> jump out
         addInstruction(VirtualInstruction::eJump, flow.end);
      }

      // generate code for all bodies, with the correct labels before them
      for ( int index = 0; index < count; index++ )
      {
         const ASTCase& astcase = ast.getCase(index);

         addLabel(labels[index]);

         astcase.getBody().accept(*this);
      }
   }

   addLabel(flow.end);
   mLoopFlowStack.pop();
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

   addInstruction(VirtualInstruction::eRet, ast.hasExpression() ? 1 : 0);
}

void CodeGeneratorVisitor::visit(const ASTTry& ast)
{
   int label = allocateLabel();
   int labelCatch = allocateLabel();

   if ( ast.hasFinallyBlock() )
      addInstruction(VirtualInstruction::eEnterGuardF, labelCatch);
   else
      addInstruction(VirtualInstruction::eEnterGuard, labelCatch);

   ast.getBody().accept(*this);

   addInstruction(VirtualInstruction::eJump, label);

   // from here the code is executed when an exception did occure
   addLabel(labelCatch);
   addInstruction(VirtualInstruction::eStoreLocal, ast.getResourceIndex());

   const ASTNodes& catches = ast.getCatches();
   for ( int index = 0; index < catches.size(); index++ )
   {
      const ASTCatch& c = static_cast<const ASTCatch&>(catches[index]);

      int labelNext = allocateLabel();

      const ASTVariable& var = c.getVariable().getVariable();

      int lit = allocateLiteral(var.getType().getObjectName());
      addInstruction(VirtualInstruction::eLoadLocal, ast.getResourceIndex());
      addInstruction(VirtualInstruction::eInstanceOf, lit);
      addInstruction(VirtualInstruction::eJumpFalse, labelNext);

      // make sure the exception variable can be resolved (is a variable after all)
      ScopedScope scope(mScopeStack);
      mScopeStack.add(ScopeVariable::fromVariable(var));

      c.getBody().accept(*this);

      addInstruction(VirtualInstruction::eJump, label);
      addLabel(labelNext);
   }

   // the exception was not handled, so re-throw it
   addInstruction(VirtualInstruction::eLoadLocal, ast.getResourceIndex());
   addInstruction(VirtualInstruction::eThrow);

   // no exception occured or it was handled successfully
   addLabel(label);
   if ( ast.hasFinallyBlock() )
   {
      ast.getFinallyBlock().accept(*this);
   }

   addInstruction(VirtualInstruction::eLeaveGuard);
}

void CodeGeneratorVisitor::visit(const ASTThrow& ast)
{
   ast.getExpression().accept(*this);

   addInstruction(VirtualInstruction::eThrow);
}

void CodeGeneratorVisitor::visit(const ASTAssert& ast)
{
   ast.getCondition().accept(*this);

   int labelend = allocateLabel();
   int errorlit = allocateLiteral("system.AssertionError");

   addInstruction(VirtualInstruction::eJumpTrue, labelend);
   addInstruction(VirtualInstruction::ePush, errorlit);
   addInstruction(VirtualInstruction::eNew, -1);
   addInstruction(VirtualInstruction::eThrow);

   addLabel(labelend);
}

void CodeGeneratorVisitor::visit(const ASTLoopControl& ast)
{
   LoopFlow& info = mLoopFlowStack.top();

   // must become a stack of labels to support nested loops
   if ( ast.getKind() == ASTLoopControl::eBreak )
   {
      addInstruction(VirtualInstruction::eJump, info.end);
   }
   else
   {
      addInstruction(VirtualInstruction::eJump, info.start);
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

      ast.getLeft().accept(*this);

      mStore = false;
      mNeedPop = false;

      bool local = mpAccess->getAccess() == ASTAccess::eLocal;

      if ( mpAccess->getAccess() == ASTAccess::eField )
         addInstruction(VirtualInstruction::ePushThis);

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

   int labelResult = allocateLabel();
   int labelFinish = allocateLabel();

   switch ( concatenate.getMode() )
   {
      // arithmatic operators
      case ASTConcatenate::ePlus:
         {
            concatenate.getRight().accept(*this);

            // two operants are already same type due to cast
            if ( mCurrentType.isInt() )
               addInstruction(VirtualInstruction::eAddInt);
            else if ( mCurrentType.isReal() )
               addInstruction(VirtualInstruction::eAddReal);
            else if ( mCurrentType.isString() )
               addInstruction(VirtualInstruction::eAddStr);
         }
         break;

      case ASTConcatenate::eMinus:
         {
            concatenate.getRight().accept(*this);

            if ( mCurrentType.isInt() )
               addInstruction(VirtualInstruction::eSubInt);
            else if ( mCurrentType.isReal() )
               addInstruction(VirtualInstruction::eSubReal);
         }
         break;

      case ASTConcatenate::eMul:
         {
            concatenate.getRight().accept(*this);

            if ( mCurrentType.isInt() )
               addInstruction(VirtualInstruction::eMulInt);
            else if ( mCurrentType.isReal() )
               addInstruction(VirtualInstruction::eMulReal);
         }
         break;

      case ASTConcatenate::eDiv:
         {
            concatenate.getRight().accept(*this);

            if ( mCurrentType.isInt() )
               addInstruction(VirtualInstruction::eDivInt);
            else if ( mCurrentType.isReal() )
               addInstruction(VirtualInstruction::eDivReal);
         }
         break;

      case ASTConcatenate::eRem:
         {
            concatenate.getRight().accept(*this);

            if ( mCurrentType.isInt() )
               addInstruction(VirtualInstruction::eRemInt);
            else if ( mCurrentType.isReal() )
               addInstruction(VirtualInstruction::eRemReal);
         }
         break;

      // bit operators

      case ASTConcatenate::eBitwiseOr:
         {
            concatenate.getRight().accept(*this);

            addInstruction(VirtualInstruction::eOrInt);
         }
         break;

      case ASTConcatenate::eBitwiseXor:
         {
            concatenate.getRight().accept(*this);

            addInstruction(VirtualInstruction::eXorInt);
         }
         break;

      case ASTConcatenate::eBitwiseAnd:
         {
            concatenate.getRight().accept(*this);

            addInstruction(VirtualInstruction::eAndInt);
         }
         break;

      case ASTConcatenate::eShiftLeft:
         {
            concatenate.getRight().accept(*this);

            addInstruction(VirtualInstruction::eShiftLeftInt);
         }
         break;

      case ASTConcatenate::eShiftRight:
         {
            concatenate.getRight().accept(*this);

            addInstruction(VirtualInstruction::eShiftRightInt);
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
               case ASTType::eBoolean:
                  addInstruction(VirtualInstruction::eCmpEqBool);
                  break;
               case ASTType::eInt:
                  addInstruction(VirtualInstruction::eCmpEqInt);
                  break;
               case ASTType::eReal:
                  addInstruction(VirtualInstruction::eCmpEqReal);
                  break;
               case ASTType::eString:
                  addInstruction(VirtualInstruction::eCmpEqStr);
                  break;
               case ASTType::eObject:
                  addInstruction(VirtualInstruction::eCmpEqObj);
                  break;
               case ASTType::eArray:
                  addInstruction(VirtualInstruction::eCmpEqAr);
                  break;
               case ASTType::eNull:
                  addInstruction(VirtualInstruction::eIsNull);
                  break;
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
               case ASTType::eBoolean:
                  addInstruction(VirtualInstruction::eCmpNeqBool);
                  break;
               case ASTType::eInt:
                  addInstruction(VirtualInstruction::eCmpNeqInt);
                  break;
               case ASTType::eReal:
                  addInstruction(VirtualInstruction::eCmpNeqReal);
                  break;
               case ASTType::eString:
                  addInstruction(VirtualInstruction::eCmpNeqStr);
                  break;
               case ASTType::eObject:
                  addInstruction(VirtualInstruction::eCmpNeqObj);
                  break;
               case ASTType::eArray:
                  addInstruction(VirtualInstruction::eCmpNeqAr);
                  break;
               case ASTType::eNull:
                  addInstruction(VirtualInstruction::eIsNull);
                  addInstruction(VirtualInstruction::eNot);
                  break;
            }
         }
         break;

      case ASTConcatenate::eSmallerEqual:
         {
            concatenate.getRight().accept(*this);

            switch ( mCurrentType.getKind() )
            {
               case ASTType::eInt:
                  addInstruction(VirtualInstruction::eCmpLeInt);
                  break;
               case ASTType::eReal:
                  addInstruction(VirtualInstruction::eCmpLeReal);
                  break;
               case ASTType::eString:
                  addInstruction(VirtualInstruction::eCmpLeStr);
                  break;
            }
         }
         break;

      case ASTConcatenate::eSmaller:
         {
            concatenate.getRight().accept(*this);

            switch ( mCurrentType.getKind() )
            {
               case ASTType::eInt:
                  addInstruction(VirtualInstruction::eCmpLtInt);
                  break;
               case ASTType::eReal:
                  addInstruction(VirtualInstruction::eCmpLtReal);
                  break;
               case ASTType::eString:
                  addInstruction(VirtualInstruction::eCmpLtStr);
                  break;
            }
         }
         break;

      case ASTConcatenate::eGreater:
         {
            concatenate.getRight().accept(*this);

            switch ( mCurrentType.getKind() )
            {
               case ASTType::eInt:
                  addInstruction(VirtualInstruction::eCmpGtInt);
                  break;
               case ASTType::eReal:
                  addInstruction(VirtualInstruction::eCmpGtReal);
                  break;
               case ASTType::eString:
                  addInstruction(VirtualInstruction::eCmpGtStr);
                  break;
            }
         }
         break;

      case ASTConcatenate::eGreaterEqual:
         {
            concatenate.getRight().accept(*this);

            switch ( mCurrentType.getKind() )
            {
               case ASTType::eInt:
                  addInstruction(VirtualInstruction::eCmpGeInt);
                  break;
               case ASTType::eReal:
                  addInstruction(VirtualInstruction::eCmpGeReal);
                  break;
               case ASTType::eString:
                  addInstruction(VirtualInstruction::eCmpGeStr);
                  break;
            }
         }
         break;

      // and/or

      case ASTConcatenate::eOr:
         {
            addInstruction(VirtualInstruction::eJumpTrue, labelResult);    // true

            concatenate.getRight().accept(*this);

            addInstruction(VirtualInstruction::eJumpTrue, labelResult);    // true
            addInstruction(VirtualInstruction::ePushFalse);                // push false
            addInstruction(VirtualInstruction::eJump, labelFinish);        // jump to finish
            addLabel(labelResult);
            addInstruction(VirtualInstruction::ePushTrue);                 // push true
            addLabel(labelFinish);
         }
         break;

      case ASTConcatenate::eAnd:
         {
            addInstruction(VirtualInstruction::eJumpFalse, labelResult);   // false

            concatenate.getRight().accept(*this);

            addInstruction(VirtualInstruction::eJumpFalse, labelResult);   // false
            addInstruction(VirtualInstruction::ePushTrue);                 // push true
            addInstruction(VirtualInstruction::eJump, labelFinish);        // jump to finish
            addLabel(labelResult);
            addInstruction(VirtualInstruction::ePushFalse);                // push false
            addLabel(labelFinish);
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
         addInstruction(VirtualInstruction::ePushThis);
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
         mLoadFlags |= eKeep; // determine if the result is still used!!
      }

      children[index].accept(*this);

      mLoadFlags = 0;

      switch ( ast.getPre() )
      {
         case ASTUnary::eNegate:
            {
               if ( mCurrentType.isInt() )
                  addInstruction(VirtualInstruction::eIntNegate);
               else if ( mCurrentType.isReal() )
                  addInstruction(VirtualInstruction::eRealNegate);
            }
            break;
         case ASTUnary::eNot:
            {
               addInstruction(VirtualInstruction::eNot);
            }
            break;
      }
   }
}

void CodeGeneratorVisitor::visit(const ASTInstanceOf& ast)
{
   ast.getObject().accept(*this);

   int typenameid = allocateLiteral(ast.getInstanceType().getObjectName());
   addInstruction(VirtualInstruction::eInstanceOf, typenameid);
}

void CodeGeneratorVisitor::visit(const ASTNew& ast)
{
   switch ( ast.getKind() )
   {
      case ASTNew::eObject:
         {
            int typenameid = allocateLiteral(ast.getType().getObjectName());

            mCurrentType = ast.getType();

            ast.getArguments().accept(*this);

            addInstruction(VirtualInstruction::ePush, typenameid);
            addInstruction(VirtualInstruction::eNew, ast.getConstructor().getResourceIndex()); // constructor index as argument
         }
         break;

      case ASTNew::eArray:
         {
            // reverse put sizes on stack
            reverseVisitChildren(ast);

            addInstruction(VirtualInstruction::eNewArray, ast.getArrayDimension());
         }
         break;
   }
}

void CodeGeneratorVisitor::visit(const ASTSuper& ast)
{
   if ( ast.hasConstructor() )
   {
      addInstruction(VirtualInstruction::ePushThis);

      // call to constructor of superclass
      visitChildren(ast);

      addInstruction(VirtualInstruction::ePush, ast.getArgumentCount() + 1);
      addInstruction(ast.isSuper() ? VirtualInstruction::eCallSuper : VirtualInstruction::eCall, ast.getConstructor().getResourceIndex());

      mNeedPop = false;
   }
   else if ( ast.isThis() )
   {
      addInstruction(VirtualInstruction::ePushThis);
   }
   else
   {
      mSuperCall = true;
   }
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
            if ( from.isReal() )
            {
               addInstruction(VirtualInstruction::eReal2Int);
            }
            break;
         case ASTType::eReal:
            if ( from.isInt() )
            {
               addInstruction(VirtualInstruction::eInt2Real);
            }
            break;
         case ASTType::eString:
            if ( from.isInt() )
               addInstruction(VirtualInstruction::eInt2String);
            else if ( from.isReal() )
               addInstruction(VirtualInstruction::eReal2String);
            else if ( from.isBoolean() )
               addInstruction(VirtualInstruction::eBoolean2String);
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
                     int classlit = allocateLiteral(mCurrentType.isValid() ? mCurrentType.getObjectClass().getFullName() : mpClass->getFullName());
                     addInstruction(VirtualInstruction::ePush, classlit);
                  }
                  else
                  {
                     addInstruction(VirtualInstruction::ePushThis);
                  }

                  handleVariable(var, false);
                  break;

               case ASTAccess::eRefField:
                  if ( isstatic )
                  {
                     addInstruction(VirtualInstruction::ePush, allocateLiteral(mCurrentType.getObjectClass().getFullName()));
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
               mCurrentType = mCurrentType.getTypeArguments()[typevariable.getIndex()];
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

            addInstruction(VirtualInstruction::eLoadArray, ast.getArguments().size());
         }
         break;

      case ASTAccess::eFunction:
         {
            const ASTFunction& function = ast.getFunction();

            if ( !mCurrentType.isValid() )
            {
               addInstruction(VirtualInstruction::ePushThis);
            }

            ASTType before = mCurrentType;

            visitChildren(ast);

            addInstruction(VirtualInstruction::ePush, function.getArgumentCount());

            if ( function.getModifiers().isNative() )
            {
               addInstruction(VirtualInstruction::eCall, function.getResourceIndex());
            }
            else if ( function.getModifiers().isStatic() )
            {
               addInstruction(VirtualInstruction::ePush, allocateLiteral(before.isValid() ? before.getObjectClass().getFullName() : mpClass->getFullName()));
               addInstruction(VirtualInstruction::eCallStatic, function.getResourceIndex());
            }
            else if ( before.isObject() && before.getObjectClass().getKind() == ASTClass::eInterface )
            {
               addInstruction(VirtualInstruction::eCallInterface, function.getResourceIndex());
            }
            else if ( mSuperCall )
            {
               addInstruction(VirtualInstruction::eCallSuper, function.getResourceIndex());
            }
            else
            {
               addInstruction(VirtualInstruction::eCall, function.getResourceIndex());
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
               addInstruction(VirtualInstruction::eLoadClass, 1);
            }
            else
            {
               addInstruction(VirtualInstruction::ePush, allocateLiteral(mCurrentType.getObjectClass().getFullName()));
               addInstruction(VirtualInstruction::eLoadClass, 0);
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
   int index = ast.getLiteral().getTableIndex();

   if ( ast.getType().isNull() )
   {
      if ( !IS_SET(mState, eStateNoNull) )
      {
         addInstruction(VirtualInstruction::ePushNull);
      }
   }
   else
   {
      addInstruction(VirtualInstruction::eLoadLiteral, index);
   }

   mCurrentType = ast.getType();
}

// - Helpers

void CodeGeneratorVisitor::handleAssignment(const ASTAccess& access, bool local)
{
   switch ( access.getKind() )
   {
      case ASTAccess::eField:
         {
            bool isstatic = access.getVariable().getModifiers().isStatic();

            // the literal of the class has already been stored on the stack (see ASTAccess::eStatic and unary)

            int instruction = local ? VirtualInstruction::eStoreLocal : (isstatic ? VirtualInstruction::eStoreStatic : VirtualInstruction::eStore);
            addInstruction(instruction, access.getVariable().getResourceIndex());
         }
         break;

      case ASTAccess::eArray:
         {
            // add indices on stack
            reverseVisitChildren(access);

            addInstruction(VirtualInstruction::eStoreArray, access.getArguments().size());
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

   int store = local ? VirtualInstruction::eStoreLocal : (isstatic ? VirtualInstruction::eStoreStatic : VirtualInstruction::eStore);
   int load  = local ? VirtualInstruction::eLoadLocal  : (isstatic ? VirtualInstruction::eLoadStatic : VirtualInstruction::eLoad);

   if ( mLoadFlags > 0 )
   {
      // if not local, the object is already on the stack
      addInstruction(load, variable.getResourceIndex());

      if ( variable.getType().isInt() )
      {
         addInstruction(VirtualInstruction::eInt1);
         addInstruction((mLoadFlags & ePreIncr) == ePreIncr ? VirtualInstruction::eAddInt : VirtualInstruction::eSubInt);
      }
      else
      {
         addInstruction(VirtualInstruction::eReal1);
         addInstruction((mLoadFlags & ePreIncr) == ePreIncr ? VirtualInstruction::eAddReal : VirtualInstruction::eSubReal);
      }

      addInstruction(VirtualInstruction::eDup);

      int flags = mLoadFlags;
      mLoadFlags = 0;

      if ( !local )
      {
         // the unary will push this when required, after the visit an object is on top of the stack
         mRightHandSide = true;
         visit(*mpExpression);
         mRightHandSide = false;
      }

      addInstruction(store, variable.getResourceIndex());

      if ( flags > eKeep )
      {
         // revert if it should be a post operator
         if ( variable.getType().isInt() )
         {
            addInstruction(VirtualInstruction::eInt1);
            addInstruction((flags & ePostIncr) == ePostIncr ? VirtualInstruction::eSubInt : VirtualInstruction::eAddInt);
         }
         else
         {
            addInstruction(VirtualInstruction::eReal1);
            addInstruction((flags & ePostIncr) == ePostIncr ? VirtualInstruction::eSubReal : VirtualInstruction::eAddReal);
         }
      }
   }
   else
   {
      addInstruction(load, variable.getResourceIndex());
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

   int lit = allocateLiteral(ast.getFullName());

   const ASTClass::Fields& fields = ast.getStatics();
   for ( std::size_t index = 0; index < fields.size(); index++ )
   {
      const ASTField* pfield = fields[index];
      const ASTVariable& variable = pfield->getVariable();

      if ( variable.hasInit() )
      {
         mCurrentType.clear();

         const ASTVariableInit& varinit = variable.getInit();
         varinit.getExpression().accept(*this);
         addInstruction(VirtualInstruction::ePush, lit);
         addInstruction(VirtualInstruction::eStoreStatic, variable.getResourceIndex());
      }
      else
      {
         const ASTType& type = variable.getType();
         switch ( type.getKind() )
         {
            case ASTType::eBoolean:
               addInstruction(VirtualInstruction::ePushFalse);
               addInstruction(VirtualInstruction::ePush, lit);
               addInstruction(VirtualInstruction::eStoreStatic, variable.getResourceIndex());
               break;
            case ASTType::eInt:
               addInstruction(VirtualInstruction::eInt0);
               addInstruction(VirtualInstruction::ePush, lit);
               addInstruction(VirtualInstruction::eStoreStatic, variable.getResourceIndex());
               break;
            case ASTType::eReal:
               addInstruction(VirtualInstruction::eReal0);
               addInstruction(VirtualInstruction::ePush, lit);
               addInstruction(VirtualInstruction::eStoreStatic, variable.getResourceIndex());
               break;
            case ASTType::eString:
               addInstruction(VirtualInstruction::eLoadLiteral, allocateLiteral(""));
               addInstruction(VirtualInstruction::ePush, lit);
               addInstruction(VirtualInstruction::eStoreStatic, variable.getResourceIndex());
               break;
         }
      }
   }
   addInstruction(VirtualInstruction::eRet, 0);
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
      addInstruction(VirtualInstruction::ePushThis);
      addInstruction(VirtualInstruction::ePush, 1);
      addInstruction(VirtualInstruction::eCallSuper, 1);
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
         varinit.getExpression().accept(*this);

         addInstruction(VirtualInstruction::ePushThis, variable.getResourceIndex());
         addInstruction(VirtualInstruction::eStore, variable.getResourceIndex());
      }
      else
      {
         const ASTType& type = variable.getType();
         switch ( type.getKind() )
         {
            case ASTType::eBoolean:
               addInstruction(VirtualInstruction::ePushFalse);
               addInstruction(VirtualInstruction::ePushThis);
               addInstruction(VirtualInstruction::eStore, variable.getResourceIndex());
               break;
            case ASTType::eInt:
               addInstruction(VirtualInstruction::eInt0);
               addInstruction(VirtualInstruction::ePushThis);
               addInstruction(VirtualInstruction::eStore, variable.getResourceIndex());
               break;
            case ASTType::eReal:
               addInstruction(VirtualInstruction::eReal0);
               addInstruction(VirtualInstruction::ePushThis);
               addInstruction(VirtualInstruction::eStore, variable.getResourceIndex());
               break;
            case ASTType::eString:
               {
                  int lit = allocateLiteral("");
                  addInstruction(VirtualInstruction::eLoadLiteral, lit);
                  addInstruction(VirtualInstruction::ePushThis);
                  addInstruction(VirtualInstruction::eStore, variable.getResourceIndex());
               }
               break;
         }
      }
   }
   addInstruction(VirtualInstruction::eRet, 0);
}

void CodeGeneratorVisitor::handleClassObject(const ASTClass& ast)
{
   const FunctionTable& table = ast.getFunctionTable();
   VirtualArrayObject* pfuncarray = new VirtualArrayObject();
   pfuncarray->addLevel(table.size());

   for ( int index = 0; index < table.size(); index++ )
   {
      const ASTFunction& function = table[index];

      VirtualArrayObject* pannoarray = new VirtualArrayObject();
      if ( function.hasAnnotations() )
      {
         const ASTAnnotations& annotations = function.getAnnotations();
         pannoarray->addLevel(annotations.size());
         for ( int a = 0; a < annotations.size(); a++ )
         {
            const ASTAnnotation& annotation = annotations[a];

            (*pannoarray)[a] = Variant(annotation.mName);
         }
      }
      else
      {
         pannoarray->addLevel(0);
      }

      VirtualObject* funcobject = new VirtualObject();
      funcobject->initialize(2);
      funcobject->setMember(0, Variant(function.getName()));
      funcobject->setMember(1, Variant(VirtualArrayReference(pannoarray)));

      (*pfuncarray)[index] = Variant(VirtualObjectReference(funcobject));
   }

   VirtualObject* classobject = new VirtualObject();
   classobject->initialize(2);
   classobject->setMember(0, Variant(ast.getFullName()));
   classobject->setMember(1, Variant(VirtualArrayReference(pfuncarray)));

   mpVClass->setClassObject(VirtualObjectReference(classobject));
}
