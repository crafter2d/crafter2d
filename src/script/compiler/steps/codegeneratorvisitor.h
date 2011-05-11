
#ifndef CODE_GENERATOR_VISITOR_H_
#define CODE_GENERATOR_VISITOR_H_

#include "../compilestep.h"

#include <string>
#include <vector>
#include <stack>
#include <map>

#include "script/ast/asttype.h"
#include "script/scope/scopestack.h"

class ASTVariable;
class CompileContext;
class VirtualClass;
class VirtualFunction;
class VirtualProgram;

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
   virtual void visit(const ASTIf& ast);
   virtual void visit(const ASTFor& ast);
   virtual void visit(const ASTForeach& ast);
   virtual void visit(const ASTWhile& ast);
   virtual void visit(const ASTDo& ast);
   virtual void visit(const ASTReturn& ast);
   virtual void visit(const ASTTry& ast);
   virtual void visit(const ASTThrow& ast);
   virtual void visit(const ASTLoopControl& ast);
   virtual void visit(const ASTCompound& compound);
   virtual void visit(const ASTExpression& ast);
   virtual void visit(const ASTConcatenate& concatenate);
   virtual void visit(const ASTUnary& ast);
   virtual void visit(const ASTNew& ast);
   virtual void visit(const ASTSuper& ast);
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

   typedef std::stack<LoopFlow> LoopFlowStack;
   typedef std::map<std::string, const ASTLocalVariable*> VariableMap;
   typedef std::vector<Inst> InstructionList;

   enum LoadFlags { ePreIncr = 1, ePreDecr = 2, ePostIncr = 5, ePostDecr = 6, eKeep = 8 };

 // operations
   void addInstruction(int instruction, int argument = -1);
   void addLabel(int id);

   int allocateLabel();
   int allocateLiteral(const std::string& value);

   void convertLabels();
   void removeLabels();

   int findLabel(int label) const;

   void handleAssignment(const ASTAccess& access, bool local);
   void handleVariable(const ASTVariable& variable, bool local);
   void handleStaticBlock(const ASTClass& ast);
   void handleFieldBlock(const ASTClass& ast);
   void handleClassObject(const ASTClass& ast);

   void fillInstructionList();

   CompileContext&      mContext;
   const ASTClass*      mpClass;
   const ASTFunction*   mpFunction;
   const ASTAccess*     mpAccess;
   const ASTExpression* mpExpression;
   ASTType              mCurrentType;

   VirtualClass*        mpVClass;
   InstructionList      mInstructions;
   ScopeStack           mScopeStack;
   LoopFlowStack        mLoopFlowStack;        
   int                  mLabel;
   int                  mLineNr;
   int                  mLoadFlags;
   int                  mExpr;
   bool                 mSuperCall;
   bool                 mLocal;
   bool                 mRightHandSide;
   bool                 mStore;
   bool                 mNeedPop;
};

#endif // CODE_GENERATOR_VISITOR_H_
