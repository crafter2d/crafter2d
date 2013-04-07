
#ifndef CODE_GENERATOR_VISITOR_H_
#define CODE_GENERATOR_VISITOR_H_

#include "../compilestep.h"

#include <vector>
#include <stack>
#include <map>

#include "core/string/string.h"

#include "script/ast/asttype.h"
#include "script/scope/scopestack.h"
#include "script/vm/virtualinstruction.h"
#include "script/cil/cil.h"
#include "script/compiler/functionbuilder.h"

class ASTVariable;
class CodePatch;
class CompileContext;
class Literal;
class VirtualClass;
class VirtualFunction;
class VirtualLookupTable;
class CompiledClass;
class CompiledFunction;

class CodeGeneratorVisitor : public CompileStep
{
public:
   CodeGeneratorVisitor(CompileContext& vcontext);

 // operations
   void print();
   
 // visitor
   virtual void visit(const ASTRoot& root);
   virtual void visit(const ASTClass& ast);
   virtual void visit(const ASTFunction& ast);
   virtual void visit(const ASTBlock& ast);
   virtual void visit(const ASTExpressionStatement& ast);
   virtual void visit(const ASTLocalVariable& ast);
   virtual void visit(const ASTVariableInit& ast);
   virtual void visit(const ASTArrayInit& ast);
   virtual void visit(const ASTIf& ast);
   virtual void visit(const ASTFor& ast);
   virtual void visit(const ASTForeach& ast);
   virtual void visit(const ASTWhile& ast);
   virtual void visit(const ASTDo& ast);
   virtual void visit(const ASTSwitch& ast);
   virtual void visit(const ASTCase& ast);
   virtual void visit(const ASTReturn& ast);
   virtual void visit(const ASTTry& ast);
   virtual void visit(const ASTThrow& ast);
   virtual void visit(const ASTAssert& ast);
   virtual void visit(const ASTLoopControl& ast);
   virtual void visit(const ASTCompound& compound);
   virtual void visit(const ASTExpression& ast);
   virtual void visit(const ASTConcatenate& concatenate);
   virtual void visit(const ASTUnary& ast);
   virtual void visit(const ASTInstanceOf& ast);
   virtual void visit(const ASTNew& ast);
   virtual void visit(const ASTSuper& ast);
   virtual void visit(const ASTNative& ast);
   virtual void visit(const ASTCast& ast);
   virtual void visit(const ASTAccess& ast);
   virtual void visit(const ASTLiteral& literal);   

protected:
 // overloads
   virtual bool performStep(ASTNode& node);

private:
   struct Inst
   {
      int instruction;
      int arg;
      int linenr;
   };

   struct LoopFlow
   {
      int start;
      int end;
   };

   enum State
   {
      eStateNoNull = 1
   };

   typedef std::stack<LoopFlow> LoopFlowStack;
   typedef std::vector<Inst> InstructionList;
   typedef std::vector<CodePatch*> Patches;

   enum LoadFlags { ePreIncr = 1, ePreDecr = 2, ePostIncr = 5, ePostDecr = 6, eKeep = 8 };

 // operations
   void handleAssignment(const ASTAccess& access, bool local);
   void handleVariable(const ASTVariable& variable, bool local);
   void handleStaticBlock(const ASTClass& ast);
   void handleFieldBlock(const ASTClass& ast);
   void handleClassObject(const ASTClass& ast);
   void handleLiteral(const Literal& literal);

   CIL::Type* TypeToCILType(const ASTType& type);

   void addPatch(CodePatch* ppatch);
   void applyPatches();
   
   CompileContext&      mContext;
   const ASTClass*      mpClass;
   const ASTFunction*   mpFunction;
   const ASTAccess*     mpAccess;
   const ASTExpression* mpExpression;
   ASTType              mCurrentType;
   Patches              mPatches;

   CompiledClass*       mpCClass;
   CompiledFunction*    mpCFunction;
   FunctionBuilder      mBuilder;

   VirtualClass*        mpVClass;
   VirtualFunction*     mpVirFunction;
   InstructionList      mInstructions;
   ScopeStack           mScopeStack;
   LoopFlowStack        mLoopFlowStack;
   VirtualLookupTable*  mpLookupTable;
   int                  mLabel;
   int                  mLineNr;
   int                  mLoadFlags;
   int                  mExpr;
   int                  mState;
   bool                 mSuperCall;
   bool                 mRightHandSide;
   bool                 mStore;
};

#endif // CODE_GENERATOR_VISITOR_H_
