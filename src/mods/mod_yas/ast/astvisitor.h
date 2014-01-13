
#ifndef AST_VISITOR_H_
#define AST_VISITOR_H_

class ASTNode;
class ASTRoot;
class ASTPackage;
class ASTUse;
class ASTClass;
class ASTField;
class ASTProperty;
class ASTFunction;
class ASTFunctionArgument;
class ASTLocalVariable;
class ASTAnnotations;
class ASTBlock;
class ASTArrayInit;
class ASTVariableInit;
class ASTIf;
class ASTFor;
class ASTForeach;
class ASTWhile;
class ASTDo;
class ASTSwitch;
class ASTCase;
class ASTReturn;
class ASTTry;
class ASTCatch;
class ASTThrow;
class ASTAssert;
class ASTLoopControl;
class ASTExpressionStatement;
class ASTExpression;
class ASTCast;
class ASTUnary;
class ASTInstanceOf;
class ASTNew;
class ASTSuper;
class ASTNative;
class ASTAccess;
class ASTCompound;
class ASTConcatenate;
class ASTLiteral;

#define VISIT(aclass) \
   virtual void visit(const aclass& ast); \
   virtual void visit(aclass& ast)

#define VISIT_IMPL(aclass) \
   void ASTVisitor::visit(const aclass& ast) {} \
   void ASTVisitor::visit(aclass& ast) {}

class ASTVisitor
{
public:
   ASTVisitor();
   virtual ~ASTVisitor() = 0;

   VISIT(ASTRoot);
   VISIT(ASTPackage);
   VISIT(ASTUse);
   VISIT(ASTClass);
   VISIT(ASTField);
   VISIT(ASTProperty);
   VISIT(ASTFunction);
   VISIT(ASTFunctionArgument);
   VISIT(ASTLocalVariable);
   VISIT(ASTAnnotations);
   VISIT(ASTArrayInit);
   VISIT(ASTVariableInit);
   VISIT(ASTBlock);
   VISIT(ASTIf);
   VISIT(ASTFor);
   VISIT(ASTForeach);
   VISIT(ASTWhile);
   VISIT(ASTDo);
   VISIT(ASTSwitch);
   VISIT(ASTCase);
   VISIT(ASTReturn);
   VISIT(ASTTry);
   VISIT(ASTCatch);
   VISIT(ASTThrow);
   VISIT(ASTAssert);
   VISIT(ASTLoopControl);
   VISIT(ASTLiteral);
   VISIT(ASTExpressionStatement);
   VISIT(ASTExpression);
   VISIT(ASTCast);
   VISIT(ASTUnary);
   VISIT(ASTInstanceOf);
   VISIT(ASTNew);
   VISIT(ASTSuper);
   VISIT(ASTNative);
   VISIT(ASTAccess);
   VISIT(ASTCompound);
   VISIT(ASTConcatenate);

protected:
   void visitChildren(const ASTNode& node);
   void visitChildren(ASTNode& node);

   void reverseVisitChildren(const ASTNode& node);
};

#endif // AST_VISITOR_H_
