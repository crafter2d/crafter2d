
#include "codegeneratorvisitor.h"

#include <libcil/cil/class.h>
#include <libcil/cil/field.h>
#include <libcil/cil/function.h>
#include <libcil/cil/guard.h>
#include <libcil/cil/switchtabel.h>
#include <libcil/cil/switchtableentry.h>
#include <libcil/common/type.h>

#include "core/defines.h"
#include "core/string/stringinterface.h"
#include "core/system/variant.h"

#include "yasc/compiler/ast/ast.h"
#include "yasc/compiler/scope/scope.h"
#include "yasc/compiler/scope/scopevariable.h"
#include "yasc/compiler/scope/scopedscope.h"
#include "yasc/compiler/compilecontext.h"

#include "yasc/common/literal.h"

using namespace CIL;

CodeGeneratorVisitor::CodeGeneratorVisitor(CompileContext& context):
   CompileStep(context),
   mpFunction(nullptr),
   mpAccess(nullptr),
   mpExpression(nullptr),
   mCurrentType(),
   mpCilClass(nullptr),
   mpSwitchTable(nullptr),
   mScopeStack(),
   mLoopFlowStack(),
   mLoadFlags(0),
   mExpr(0),
   mState(0),
   mRightHandSide(false),
   mStore(false),
   mWasSuper(false)
{
}

// - Overloads

bool CodeGeneratorVisitor::performStep(ASTNode& node)
{
   node.accept(*this);

   return true;
}

// - Visitor

void CodeGeneratorVisitor::visit(ASTRoot& root)
{
   visitChildren(root);
}

void CodeGeneratorVisitor::visit(ASTClass& ast)
{
   CIL::Class* pclass = toCilClass(ast);
   mpCilClass = pclass;

   mpClass = &ast;
   mpClass->setCompiledClass(pclass);

   // build the special initialization blocks

   handleStaticBlock(ast);
   handleFieldBlock(ast);
   
   // compile the class

   ScopedScope scope(mScopeStack);
   visitChildren(ast);
}

void CodeGeneratorVisitor::visit(ASTFunction& ast)
{
   CIL::Function* pfunction = new CIL::Function();
   pfunction->setName(ast.getName());
   pfunction->setReturnType(toCilType(ast.getType()));
   pfunction->setModifiers(toCilModifiers(ast.getModifiers()));

   // copy the arguments
   const ASTNodes& nodes = ast.getArgumentNodes();
   for ( int index = 0; index < nodes.size(); ++index )
   {
      const ASTFunctionArgument& arg = static_cast<const ASTFunctionArgument&>(nodes[index]);
      pfunction->addArgument(toCilType(arg.getVariable().getType()));
   }

   if ( ast.hasAnnotations() )
   {
      const ASTAnnotations& annotations = ast.getAnnotations();
      for ( int index = 0; index < annotations.size(); ++index )
      {
         const ASTAnnotation& anno = annotations[index];
         pfunction->addAnnotation(anno.mName);
      }
   }
   
   if ( !ast.getModifiers().isPureNative() )
   {
      mCurrentType.clear();

      if ( ast.hasBody() )
      {
         mpFunction = &ast;

         mBuilder.start();

         ScopedScope scope(mScopeStack);
         const ASTBlock& block = ast.getBody();
         block.accept(*this);

         mBuilder.end();

         // set the generated content
         pfunction->setInstructions(mBuilder.getInstructions());
         pfunction->setGuards(mBuilder.getGuards());
         pfunction->setSwitchTables(mBuilder.getSwitchTables());
      }

      // copy the locals
      const ASTTypeList& locals = ast.getLocals();
      for ( int index = 0; index < locals.size(); ++index )
      {
         const ASTType& local = locals[index];
         yasc::Type type = yasc::Type::fromString(local.toString());
         pfunction->addLocal(type);
      }
      
      // clean up the code body to save memory
      ast.cleanup();
   }

   mpClass->getCompiledClass().add(pfunction);
}

void CodeGeneratorVisitor::visit(ASTField& ast)
{
   ASTVariable& var = ast.getVariable();

   CIL::Field* pfield = new CIL::Field();
   pfield->setName(var.getName());
   pfield->setType(toCilType(var.getType()));

   if ( var.getModifiers().isStatic() )
   {
      mpCilClass->addStatic(pfield);
   }
   else
   {
      mpCilClass->addField(pfield);
   }
}

void CodeGeneratorVisitor::visit(const ASTVariableInit& ast)
{
   visitChildren(ast);
}

void CodeGeneratorVisitor::visit(const ASTArrayInit& ast)
{
   C2D_UNUSED(ast);
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
   
   ast.getExpression().accept(*this);
}

void CodeGeneratorVisitor::visit(const ASTLocalVariable& ast)
{
   const ASTVariable& var = ast.getVariable();
      
   if ( var.hasInit() )
   {
      mCurrentType.clear();

      const ASTVariableInit& varinit = var.getInit();
      if ( varinit.hasArrayInit() )
      {
         const ASTArrayInit& arrayinit = varinit.getArrayInit();

         mBuilder.emit(CIL_ldint, arrayinit.getCount());
         mBuilder.emit(CIL_newarray, var.getType().toString());
         mBuilder.emit(CIL_stloc, var.getResourceIndex());

         int count = arrayinit.getCount();
         for ( int index = 0; index < count; index++ )
         {
            const ASTVariableInit& vinit = arrayinit.getInit(index);

            vinit.getExpression().accept(*this);

            mBuilder.emit(CIL_ldloc, var.getResourceIndex());
            mBuilder.emit(CIL_ldint, index);
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

   mBuilder.emit(CIL_jump_false, labelFalse);

   ast.getStatement().accept(*this);

   if ( ast.hasElseStatement() )
   {
      int labelFinish = mBuilder.allocateLabel();
      mBuilder.emit(CIL_jump, labelFinish);

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

      mBuilder.emit(CIL_jump_false, flow.end);
   }

   ast.getBody().accept(*this);

   mBuilder.addLabel(flow.start);

   // update part
   mExpr = 0;
   visitChildren(ast);

   mBuilder.emit(CIL_jump, labelStart);
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

   const ASTVariable& iteratorvar = ast.getIteratorVariable();
   const ASTVariable& var = ast.getVariable();
   const ASTVariableInit& varinit = var.getInit();

   ASTTypeList list;
   ASTSignature signature;

   mCurrentType.clear();

   // container object is currently at top of evaluation stack
   // - ast contains index
   // - var contains object from array

   if ( iteratorvar.getType().isInt() )
   {
      const String qualifiedname = UTEXT("system.InternalArray.size()");

      // index = 0
      mBuilder.emit(CIL_ldint, 0);
      mBuilder.emit(CIL_stloc, iteratorvar.getResourceIndex());
      mBuilder.addLabel(flow.start);

      // check for the size ( index < array.length() )
      varinit.getExpression().accept(*this);
      mBuilder.emit(CIL_call_native, qualifiedname);
      mBuilder.emit(CIL_ldloc, iteratorvar.getResourceIndex());
      mBuilder.emit(CIL_cmpeq);
      mBuilder.emit(CIL_jump_true, flow.end);
      
      // get item & execute body ( var = array[index]; )
      varinit.getExpression().accept(*this);
      mBuilder.emit(CIL_ldloc, iteratorvar.getResourceIndex());
      mBuilder.emit(CIL_ldelem, 1);
      mBuilder.emit(CIL_stloc, var.getResourceIndex());

      ast.getBody().accept(*this);

      // increment iteration ( index++; )
      mBuilder.emit(CIL_ldloc, iteratorvar.getResourceIndex());
      mBuilder.emit(CIL_ldint, 1);
      mBuilder.emit(CIL_add);
      mBuilder.emit(CIL_stloc, iteratorvar.getResourceIndex());
      mBuilder.emit(CIL_jump, flow.start);

      mExpr = 0;

      // and of loop (pop array from stack)
      mBuilder.addLabel(flow.end);
   }
   else
   {
      varinit.getExpression().accept(*this);
      String name = mCurrentType.getObjectClass().getFullName() + UTEXT(".iterator()");

      // ast = iterator
      // var = object in the container

      mBuilder.emit(CIL_call, name);
      mBuilder.emit(CIL_stloc, iteratorvar.getResourceIndex());

      mBuilder.addLabel(flow.start);

      static const String hasnextname = UTEXT("engine.collections.Iterator.hasNext()");
      static const String nextname = UTEXT("engine.collections.Iterator.next()");

      // is there still an item: if not it.hasNext goto end
      mBuilder.emit(CIL_ldloc, iteratorvar.getResourceIndex());
      mBuilder.emit(CIL_call_virt, hasnextname);
      mBuilder.emit(CIL_jump_false, flow.end);

      // store the next item from the list: var = it.next()
      mBuilder.emit(CIL_ldloc, iteratorvar.getResourceIndex());
      mBuilder.emit(CIL_call_virt, nextname);
      mBuilder.emit(CIL_stloc, var.getResourceIndex());

      ast.getBody().accept(*this);

      mBuilder.emit(CIL_jump, flow.start);
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

   mBuilder.emit(CIL_jump_false, flow.end);

   ast.getBody().accept(*this);

   mBuilder.emit(CIL_jump, flow.start);
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

   mBuilder.emit(CIL_jump_true, flow.start);
   mBuilder.addLabel(flow.end);

   mLoopFlowStack.pop();
}

void CodeGeneratorVisitor::visit(const ASTSwitch& ast)
{
   LoopFlow flow;
   flow.start = -1;
   flow.end   = mBuilder.allocateLabel();
   mLoopFlowStack.push(flow);

   if ( ast.canLookup() )
   {
      // create a lookup table for the values (filled by the cases)
      mpSwitchTable = new CIL::SwitchTable();
      mpSwitchTable->addEnd(flow.end);

      ast.getExpression().accept(*this);

      // lookup the value in the table and jump there
      // if not found -> jump to default or skip in case no default is present
      int index = mBuilder.addTable(mpSwitchTable);
      mBuilder.emit(CIL_switch, index);

      visitChildren(ast);
   }
   else
   {
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
         mBuilder.emit(CIL_jump, flow.end);
      }

      // generate code for all bodies, with the correct labels before them
      for ( int index = 0; index < count; index++ )
      {
         const ASTCase& astcase = ast.getCase(index);

         mBuilder.addLabel(labels[index]);

         astcase.getBody().accept(*this);
      }
   }

   mBuilder.addLabel(flow.end);
   mLoopFlowStack.pop();
}

void CodeGeneratorVisitor::visit(const ASTCase& ast)
{
   ASSERT(ast.isDefault() || ast.hasValue());
   ASSERT_PTR(mpSwitchTable);

   int label = mBuilder.allocateLabel();
   mBuilder.addLabel(label);

   if ( ast.isCase() )
   {
      CIL::SwitchTableEntry entry;
      entry.label = label;
      entry.value = ast.getValue();

      mpSwitchTable->add(entry);
   }
   else
   {
      mpSwitchTable->addDefault(label);
   }

   ast.getBody().accept(*this);
}

void CodeGeneratorVisitor::visit(const ASTReturn& ast)
{
   if ( ast.hasExpression() )
   {
      ast.getExpression().accept(*this);
   }

   mBuilder.emit(CIL_ret, ast.hasExpression() ? 1 : 0);
}

void CodeGeneratorVisitor::visit(const ASTTry& ast)
{
   int labelGuard = mBuilder.allocateLabel();
   int labelFinal = mBuilder.allocateLabel();
   int labelEnd   = mBuilder.allocateLabel();
   int labelCatch = mBuilder.allocateLabel();

   CIL::Guard* pguard = new CIL::Guard();
   mBuilder.addGuard(pguard);

   // set up guard structure
   pguard->finalize = ast.hasFinallyBlock();
   pguard->labels[0] = labelGuard;
   pguard->labels[1] = labelCatch;
   pguard->labels[2] = labelFinal;
   pguard->labels[3] = labelEnd;

   // mark the start of the guard, needed to determine which guard to use at runtime
   mBuilder.addLabel(labelGuard);

   ast.getBody().accept(*this);

   // if we get here, no exception happend
   mBuilder.emit(CIL_jump, labelFinal);

   // from here the code is executed when an exception did occure
   mBuilder.addLabel(labelCatch);
   mBuilder.emit(CIL_stloc, ast.getResourceIndex());

   const ASTNodes& catches = ast.getCatches();
   for ( int index = 0; index < catches.size(); index++ )
   {
      const ASTCatch& c = static_cast<const ASTCatch&>(catches[index]);

      int labelNext = mBuilder.allocateLabel();

      const ASTVariable& var = c.getVariable().getVariable();

      // jump to next if exception is not instanceof catch type
      mBuilder.emit(CIL_ldloc, ast.getResourceIndex());
      mBuilder.emit(CIL_instanceof, var.getType().getObjectClass().getFullName());
      mBuilder.emit(CIL_jump_false, labelNext);

      // make sure the exception variable can be resolved (is a variable after all)
      ScopedScope scope(mScopeStack);
      mScopeStack.add(ScopeVariable::fromVariable(var));

      c.getBody().accept(*this);

      mBuilder.emit(CIL_jump, labelFinal);
      mBuilder.addLabel(labelNext);
   }

   // the exception was not handled, so re-throw it
   mBuilder.emit(CIL_ldloc, ast.getResourceIndex());
   mBuilder.emit(CIL_throw);

   // no exception occured or it was handled successfully
   mBuilder.addLabel(labelFinal);
   if ( ast.hasFinallyBlock() )
   {
      ast.getFinallyBlock().accept(*this);
   }

   mBuilder.addLabel(labelEnd);
}

void CodeGeneratorVisitor::visit(const ASTThrow& ast)
{
   ast.getExpression().accept(*this);

   mBuilder.emit(CIL_throw);
}

void CodeGeneratorVisitor::visit(const ASTAssert& ast)
{
   ast.getCondition().accept(*this);

   int labelend = mBuilder.allocateLabel();
   String assertclass = UTEXT("system.AssertionError.AssertionError()");

   // if condition is false we throw an exception of type AssertionError,
   // else we jump to the end
   mBuilder.emit(CIL_jump_true, labelend);
   mBuilder.emit(CIL_new, assertclass);
   mBuilder.emit(CIL_throw);

   mBuilder.addLabel(labelend);
}

void CodeGeneratorVisitor::visit(const ASTLoopControl& ast)
{
   LoopFlow& info = mLoopFlowStack.top();

   // must become a stack of labels to support nested loops
   if ( ast.getKind() == ASTLoopControl::eBreak )
   {
      mBuilder.emit(CIL_jump, info.end);
   }
   else
   {
      mBuilder.emit(CIL_jump, info.start);
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

      bool calcassign = ast.getKind() == ASTExpression::ePlusAssign || ast.getKind() == ASTExpression::eMinAssign;
      if ( calcassign )
      {
         // load the value to add
         ast.getLeft().accept(*this);
      }

      ast.getRight().accept(*this);

      if ( calcassign )
      {
         if ( ast.getKind() == ASTExpression::ePlusAssign )
            mBuilder.emit(CIL_add);
         else
            mBuilder.emit(CIL_sub);
      }

      mStore = true;

      mCurrentType.clear();
      ast.getLeft().accept(*this);

      mStore = false;

      ASSERT_PTR(mpAccess);
      ASSERT(mpAccess->getAccess() != ASTAccess::eInvalidAccess);
      handleAssignment(*mpAccess);
   }
   else
   {
      mpExpression = &ast;
      
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

            mBuilder.emit(CIL_add);
         }
         break;

      case ASTConcatenate::eMinus:
         {
            concatenate.getRight().accept(*this);

            mBuilder.emit(CIL_sub);
         }
         break;

      case ASTConcatenate::eMul:
         {
            concatenate.getRight().accept(*this);

            mBuilder.emit(CIL_mul);
         }
         break;

      case ASTConcatenate::eDiv:
         {
            concatenate.getRight().accept(*this);

            mBuilder.emit(CIL_div);
         }
         break;

      case ASTConcatenate::eRem:
         {
            concatenate.getRight().accept(*this);

            mBuilder.emit(CIL_rem);
         }
         break;

      // bit operators

      case ASTConcatenate::eBitwiseOr:
         {
            concatenate.getRight().accept(*this);

            mBuilder.emit(CIL_or);
         }
         break;

      case ASTConcatenate::eBitwiseXor:
         {
            concatenate.getRight().accept(*this);

            mBuilder.emit(CIL_xor);
         }
         break;

      case ASTConcatenate::eBitwiseAnd:
         {
            concatenate.getRight().accept(*this);

            mBuilder.emit(CIL_and);
         }
         break;

      case ASTConcatenate::eShiftLeft:
         {
            concatenate.getRight().accept(*this);

            mBuilder.emit(CIL_shl);
         }
         break;

      case ASTConcatenate::eShiftRight:
         {
            concatenate.getRight().accept(*this);

            mBuilder.emit(CIL_shr);
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
                  mBuilder.emit(CIL_isnull);
                  break;
               default:
                  mBuilder.emit(CIL_cmpeq);
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
                  mBuilder.emit(CIL_isnull);
                  mBuilder.emit(CIL_not);
                  break;
               default:
                  mBuilder.emit(CIL_cmpne);
                  break;
            }
         }
         break;

      case ASTConcatenate::eSmallerEqual:
         {
            concatenate.getRight().accept(*this);

            mBuilder.emit(CIL_cmple);
         }
         break;

      case ASTConcatenate::eSmaller:
         {
            concatenate.getRight().accept(*this);

            mBuilder.emit(CIL_cmplt);
         }
         break;

      case ASTConcatenate::eGreater:
         {
            concatenate.getRight().accept(*this);

            mBuilder.emit(CIL_cmpgt);
         }
         break;

      case ASTConcatenate::eGreaterEqual:
         {
            concatenate.getRight().accept(*this);

            mBuilder.emit(CIL_cmpge);
         }
         break;

      // and/or

      case ASTConcatenate::eOr:
         {
            mBuilder.emit(CIL_jump_true, labelResult);                  // true

            concatenate.getRight().accept(*this);

            mBuilder.emit(CIL_jump_true, labelResult);                  // true
            mBuilder.emit(CIL_ldfalse);                                 // push false
            mBuilder.emit(CIL_jump, labelFinish);                       // jump to finish
            mBuilder.addLabel(labelResult);
            mBuilder.emit(CIL_ldtrue);                                  // push true
            mBuilder.addLabel(labelFinish);
         }
         break;

      case ASTConcatenate::eAnd:
         {
            mBuilder.emit(CIL_jump_false, labelResult);                   // false

            concatenate.getRight().accept(*this);

            mBuilder.emit(CIL_jump_false, labelResult);                  // false
            mBuilder.emit(CIL_ldtrue);                                   // push true
            mBuilder.emit(CIL_jump, labelFinish);                        // jump to finish
            mBuilder.addLabel(labelResult);
            mBuilder.emit(CIL_ldfalse);                                  // push false
            mBuilder.addLabel(labelFinish);
         }
         break;
         
      default:
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
         mBuilder.emit(CIL_ldarg, 0);
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
            mBuilder.emit(CIL_neg);
            break;
         case ASTUnary::eNot:
            mBuilder.emit(CIL_not);
            break;
         default:
            break;
      }
   }
}

void CodeGeneratorVisitor::visit(const ASTInstanceOf& ast)
{
   ast.getObject().accept(*this);

   String classname = ast.getInstanceType().getObjectClass().getFullName();
   mBuilder.emit(CIL_instanceof, classname);
}

void CodeGeneratorVisitor::visit(const ASTNew& ast)
{
   switch ( ast.getKind() )
   {
      case ASTNew::eObject:
         {
            String classname = ast.getType().getObjectClass().getFullName() + '.' + ast.getConstructor().getPrototype();
            mCurrentType = ast.getType();

            ast.getArguments().accept(*this);

            mBuilder.emit(CIL_new, classname); // constructor index as argument
         }
         break;

      case ASTNew::eArray:
         {
            // reverse put sizes on stack
            reverseVisitChildren(ast);

            mBuilder.emit(CIL_newarray, ast.getType().toString());
         }
         break;
         
      case ASTNew::eInvalid:
         break;
   }
}

void CodeGeneratorVisitor::visit(const ASTSuper& ast)
{
   if ( ast.isCall() )
   {
      ASSERT(ast.hasConstructor());

      mBuilder.emit(CIL_ldarg, 0);

      // call to constructor of superclass
      visitChildren(ast);

      const ASTClass* pclass = mpClass;
      if ( ast.isSuper() )
      {
         pclass = &ast.getConstructor().getClass();
      }

      String name = pclass->getFullName() + '.' + ast.getConstructor().getPrototype();
      mBuilder.emit(CIL_call, name);
   }
   else if ( ast.isSuper() )
   {
      mWasSuper = true;
   }
   else if ( ast.isThis() )
   {
      mBuilder.emit(CIL_ldarg, 0);
   }
}

void CodeGeneratorVisitor::visit(const ASTNative& ast)
{
   mBuilder.emit(CIL_ldarg, 0);

   if ( ast.hasArguments() )
   {
      visitChildren(ast); // place arguments on the stack
   }
   else
   {
      ASSERT_PTR(mpFunction);
      int arguments = mpFunction->getArgumentCount();
      for ( int index = 1; index < arguments; index++ )
      {
         mBuilder.emit(CIL_ldarg, index);
      }
   }

   String qualifiedname = mpClass->getFullName() + L'.' + mpFunction->getPrototype();
   mBuilder.emit(CIL_call_native, qualifiedname);
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
      switch ( from.getKind() )
      {
         case ASTType::eBoolean:
            if ( mCurrentType.isString() )
               mBuilder.emit(CIL_bconv_str);
            break;
         case ASTType::eInt:
            if ( mCurrentType.isReal() )
               mBuilder.emit(CIL_iconv_real);
            else if ( mCurrentType.isString() )
               mBuilder.emit(CIL_iconv_str);
            break;
         case ASTType::eReal:
            if ( mCurrentType.isInt() )
               mBuilder.emit(CIL_rconv_int);
            else if ( mCurrentType.isString() )
               mBuilder.emit(CIL_rconv_str);
            break;
         case ASTType::eChar:
            if ( mCurrentType.isString() )
               mBuilder.emit(CIL_cconv_str);
            break;
         case ASTType::eString:
            if ( mCurrentType.isBoolean() )
               mBuilder.emit(CIL_sconv_bool);
            else if ( mCurrentType.isInt() )
               mBuilder.emit(CIL_sconv_int);
            else if ( mCurrentType.isReal() )
               mBuilder.emit(CIL_rconv_str);
            break;
         default:
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
            switch ( ast.getAccess() )
            {
               case ASTAccess::eField:
               case ASTAccess::eRefField:
                  {
                     const ASTField& field = ast.getField();

                     if ( ast.getAccess() == ASTAccess::eField && !field.getVariable().getModifiers().isStatic() )
                     {
                        // only emit loading this for current fields
                        // with reference the object is already there
                        mBuilder.emit(CIL_ldarg, 0);
                     }
                     handleField(field);
                  }
                  break;

               case ASTAccess::eArgument:
               case ASTAccess::eLocal:
                  {
                     const ASTVariable& var = ast.getVariable();
                     handleVariable(var);
                  }
                  break;
                  
               default:
                  break;
            }

            const ASTType& type = ast.getVariable().getType();
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

            ASTType before = mCurrentType;

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

            if ( mWasSuper )
            {
               String name = function.getClass().getFullName() + '.' + function.getPrototype();
               mBuilder.emit(CIL_call, name);
               mWasSuper = false;
            }
            else if ( function.getModifiers().isPureNative() )
            {
               const String qualifiedname = function.getClass().getFullName() + '.' + function.getPrototype();
               mBuilder.emit(CIL_call_native, qualifiedname);
            }
            else if ( function.getModifiers().isStatic() ) // first check for static so native statics are possible as well
            {
               String name = (before.isValid() ? before.getObjectClass().getFullName() : mpClass->getFullName()) + '.' + function.getPrototype();
               mBuilder.emit(CIL_call, name);
            }
            else 
            {
               if ( before.isObject() && before.getObjectClass().getKind() == ASTClass::eInterface )
               {
                  String name = before.getObjectClass().getFullName() + '.' + function.getPrototype();
                  mBuilder.emit(CIL_call_interface, name);
               }
               else
               {
                  String name = function.getClass().getFullName() + '.' + function.getPrototype();
                  mBuilder.emit(CIL_call_virt, name);
               }
            }

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
               mBuilder.emit(CIL_ldclass, String::empty());
            }
            else
            {
               mBuilder.emit(CIL_ldclass, mCurrentType.getObjectClass().getFullName());
            }

            mCurrentType.clear();
            mCurrentType.setKind(ASTType::eObject);
            mCurrentType.setObjectName(UTEXT("system.Class"));
            mCurrentType.setObjectClass(mContext.resolveClass(UTEXT("system.Class")));
         }
         break;

      case ASTAccess::eStatic:
         {
            mCurrentType = ast.getStaticType();

            // the function/variable access pushes it's own label.
         }
         break;
         
      case ASTAccess::eInvalid:
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
         mBuilder.emit(CIL_ldnull);
      }
   }
   else
   {
      handleLiteral(ast.getLiteral());
   }

   mCurrentType = ast.getType();
}

// - Helpers

void CodeGeneratorVisitor::handleAssignment(const ASTAccess& access)
{
   switch ( access.getKind() )
   {
      case ASTAccess::eField:
         {
            // the literal of the class has already been stored on the stack (see ASTAccess::eStatic and unary)
            

            switch ( mpAccess->getAccess() )
            {
            case ASTAccess::eLocal:
               mBuilder.emit(CIL_stloc, access.getVariable().getResourceIndex());
               break;

            case ASTAccess::eField:
            case ASTAccess::eRefField:
               {
                  const ASTField& field = access.getField();

                  bool isstatic = field.getVariable().getModifiers().isStatic();
                  if ( isstatic )
                  {
                     mBuilder.emit(CIL_ststatic, field.getQualifiedName());
                  }
                  else
                  {
                     if ( mpAccess->getAccess() == ASTAccess::eField )
                        mBuilder.emit(CIL_ldarg, 0);

                     mBuilder.emit(CIL_stfield, field.getQualifiedName());
                  }
               }
               break;
               
            default:
               break;
            }            
         }
         break;

      case ASTAccess::eArray:
         {
            // add indices on stack
            reverseVisitChildren(access);

            mBuilder.emit(CIL_stelem, access.getArguments().size());
         }
         break;

      case ASTAccess::eFunction:
         {
            // we should never get here
            UNREACHABLE("");
         }
         break;
         
      default:
         break;
   }
}

void CodeGeneratorVisitor::handleField(const ASTField& field)
{
   const ASTVariable& variable = field.getVariable();
   bool isstatic = variable.getModifiers().isStatic();

   Opcode store = (isstatic ? CIL_ststatic : CIL_stfield);
   Opcode load  = (isstatic ? CIL_ldstatic : CIL_ldfield);

   if ( mLoadFlags > 0 )
   {
      // if not local, the object is already on the stack
      mBuilder.emit(load, field.getQualifiedName());

      if ( variable.getType().isInt() )
         mBuilder.emit(CIL_ldint, 1);
      else
         mBuilder.emit(CIL_ldreal, 1.0f);

      if ( IS_SET(mLoadFlags, ePreIncr) || IS_SET(mLoadFlags, ePostIncr) )
         mBuilder.emit(CIL_add);
      else if ( IS_SET(mLoadFlags, ePreDecr) || IS_SET(mLoadFlags, ePostDecr) )
         mBuilder.emit(CIL_sub);

      if ( mLoadFlags > eKeep )
      {
         mBuilder.emit(CIL_dup);
      }

      int flags = mLoadFlags;
      mLoadFlags = 0;

      // the unary will push this when required, after the visit an object is on top of the stack
      mRightHandSide = true;
      visit(*mpExpression);
      mRightHandSide = false;

      mBuilder.emit(store, field.getQualifiedName());
      
      if ( flags > eKeep )
      {
         // revert if it should be a post operator
         if ( variable.getType().isInt() )
            mBuilder.emit(CIL_ldint, 1);
         else
            mBuilder.emit(CIL_ldreal, 1.0f);

         if ( IS_SET(flags, ePostIncr) )
            mBuilder.emit(CIL_sub);
         else if ( IS_SET(flags, ePostDecr) )
            mBuilder.emit(CIL_add);
      }
   }
   else
   {
      mBuilder.emit(load, field.getQualifiedName());
   }
}

void CodeGeneratorVisitor::handleVariable(const ASTVariable& variable)
{
   bool isargument = variable.isArgument();
   
   Opcode load  = (isargument ? CIL_ldarg : CIL_ldloc);
   mBuilder.emit(load, variable.getResourceIndex());

   if ( mLoadFlags > 0 )
   {
      if ( IS_SET(mLoadFlags, ePreIncr) || IS_SET(mLoadFlags, ePreDecr) )
      {
         // pre
		   // - increment value
         // - dup it (if required)

         if (variable.getType().isInt())
            mBuilder.emit(CIL_ldint, 1);
         else
            mBuilder.emit(CIL_ldreal, 1.0f);

         if ( IS_SET(mLoadFlags, ePreIncr) )
            mBuilder.emit(CIL_add);
         else
            mBuilder.emit(CIL_sub);

         if ( mLoadFlags > eKeep )
         {
            mBuilder.emit(CIL_dup);
         }
	   }
      else // post operator
      {
         assert(IS_SET(mLoadFlags, ePostIncr) || IS_SET(mLoadFlags, ePostDecr));

         // post
         // - dup to keep original value (if required)
         // - incr/decr value

         if ( mLoadFlags > eKeep )
         {
            mBuilder.emit(CIL_dup);
         }

         if ( variable.getType().isInt() )
            mBuilder.emit(CIL_ldint, 1);
         else
            mBuilder.emit(CIL_ldreal, 1.0f);

         if ( IS_SET(mLoadFlags, ePostIncr) )
            mBuilder.emit(CIL_add);
         else
            mBuilder.emit(CIL_sub);
      }

      Opcode store = (isargument ? CIL_ldarg : CIL_stloc);
      mBuilder.emit(store, variable.getResourceIndex());
   }
}

void CodeGeneratorVisitor::handleStaticBlock(ASTClass& ast)
{
   // var_init blocks have no return value & no arguments
   CIL::Function* pfunction = new CIL::Function();
   pfunction->setName(UTEXT("static_init"));
   pfunction->setModifiers(Modifiers(Modifiers::eStatic));
   pfunction->setReturnType(toCilType(ASTType::SVoidType));
   
   // not adding the this argument, lets see if we can do that later at runtime

   mpCilClass->add(pfunction);

   mBuilder.start();

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

            mBuilder.emit(CIL_ststatic, pfield->getQualifiedName());
         }
         else // array initializer
         {
            const ASTArrayInit& arrayinit = varinit.getArrayInit();

            // var = new array[count]
            mBuilder.emit(CIL_ldint, arrayinit.getCount());
            mBuilder.emit(CIL_newarray, variable.getType().toString());
            mBuilder.emit(CIL_ststatic, pfield->getQualifiedName());

            int count = arrayinit.getCount();
            for ( int index = 0; index < count; index++ )
            {
               const ASTVariableInit& vinit = arrayinit.getInit(index);

               vinit.getExpression().accept(*this);

               // var[index] = expr
               mBuilder.emit(CIL_ldstatic, pfield->getQualifiedName());
               mBuilder.emit(CIL_ldint, index);
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
               mBuilder.emit(CIL_ldfalse);
               break;
            case ASTType::eInt:
               mBuilder.emit(CIL_ldint, 0);
               break;
            case ASTType::eReal:
               mBuilder.emit(CIL_ldreal, 0.0f);
               break;
            case ASTType::eString:
               mBuilder.emit(CIL_ldstr, String());
               break;
            case ASTType::eObject:
            case ASTType::eArray:
               mBuilder.emit(CIL_ldnull);
               break;
               
            default:
               break;
         }

         // class.var = init
         mBuilder.emit(CIL_ststatic, pfield->getQualifiedName());
      }
   }
   mBuilder.emit(CIL_ret, 0);

   // build the function
   mBuilder.end();

   pfunction->setInstructions(mBuilder.getInstructions());
}

void CodeGeneratorVisitor::handleFieldBlock(ASTClass& ast)
{
   // construct the functions
   CIL::Function* pfunction = new CIL::Function();
   pfunction->setName(UTEXT("var_init"));
   pfunction->setReturnType(toCilType(ASTType::SVoidType));

   mpCilClass->add(pfunction);

   mBuilder.start();

   if ( ast.hasBaseClass() )
   {
      String name = ast.getBaseClass().getFullName() + UTEXT(".var_init()");
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
            mBuilder.emit(CIL_stfield, pfield->getQualifiedName());
         }
         else // array initialization (currently only for one dimensional arrays)
         {
            const ASTArrayInit& arrayinit = varinit.getArrayInit();

            mBuilder.emit(CIL_ldint, arrayinit.getCount());
            mBuilder.emit(CIL_newarray, variable.getType().toString());
            mBuilder.emit(CIL_ldarg, 0);
            mBuilder.emit(CIL_stfield, pfield->getQualifiedName());

            int count = arrayinit.getCount();
            for ( int index = 0; index < count; index++ )
            {
               const ASTVariableInit& vinit = arrayinit.getInit(index);

               vinit.getExpression().accept(*this);

               mBuilder.emit(CIL_ldarg, 0);
               mBuilder.emit(CIL_ldfield, pfield->getQualifiedName());
               mBuilder.emit(CIL_ldint, index);
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
               mBuilder.emit(CIL_ldfalse);
               break;
            case ASTType::eInt:
               mBuilder.emit(CIL_ldint, 0);
               break;
            case ASTType::eReal:
               mBuilder.emit(CIL_ldreal, 0.0f);
               break;
            case ASTType::eString:
               mBuilder.emit(CIL_ldstr, String());
               break;
            case ASTType::eObject:
            case ASTType::eArray:
            case ASTType::eGeneric:
               mBuilder.emit(CIL_ldnull);
               break;
               
            default:
               break;
         }

         mBuilder.emit(CIL_ldarg, 0);
         mBuilder.emit(CIL_stfield, pfield->getQualifiedName());
      }
   }
   mBuilder.emit(CIL_ret, 0);
   mBuilder.end();

   pfunction->setInstructions(mBuilder.getInstructions());
}

void CodeGeneratorVisitor::handleLiteral(const Literal& literal)
{
   const Variant& value = literal.getValue();
   if ( value.isInt() )
   {
      mBuilder.emit(CIL_ldint, value.asInt());
   }
   else if ( value.isReal() )
   {
      mBuilder.emit(CIL_ldreal, value.asReal());
   }
   else if ( value.isBool() )
   {
      mBuilder.emit(value.asBool() ? CIL_ldtrue : CIL_ldfalse, 0);
   }
   else if ( value.isString() )
   {
      mBuilder.emit(CIL_ldstr, value.asString());
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

// - Conversion

CIL::Class* CodeGeneratorVisitor::toCilClass(const ASTClass& klass)
{
   CIL::Class* pclass = new CIL::Class();
   pclass->setName(klass.getFullName());
   if ( klass.hasBaseClass() )
   {
      pclass->setBaseName(klass.getBaseClass().getFullName());
   }
   
   // add all interfaces
   const ASTTypeList& intfcs = klass.getInterfaces();
   for ( int index = 0; index < intfcs.size(); ++index )
   {
      const ASTType& interfce = intfcs[index];
      pclass->addInterface(interfce.toString());
   }

   return pclass;
}

yasc::Type CodeGeneratorVisitor::toCilType(const ASTType& type)
{
   String typestr = type.toString();
   return yasc::Type::fromString(typestr);
}

Modifiers CodeGeneratorVisitor::toCilModifiers(const ASTModifiers& modifiers)
{
   Modifiers mods;

   if ( modifiers.isAbstract() )
      mods.setAbstract(true);
   if ( modifiers.isPureNative() )
      mods.setPureNative(true);
   if ( modifiers.isStatic() )
      mods.setStatic(true);

   return mods;
}
