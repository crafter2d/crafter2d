
#ifndef AST_VARIABLE_INIT_H
#define AST_VARIABLE_INIT_H

class ASTArrayInit;
class ASTExpression;

class ASTVariableInit
{
public:
   ASTVariableInit();
   ~ASTVariableInit();

 // get/set
   bool                 hasExpression() const;
   const ASTExpression& getExpression() const;
         ASTExpression& getExpression();
   void                 setExpression(ASTExpression* pexpression);

   bool                 hasArrayInit() const;
   const ASTArrayInit&  getArrayInit() const;
         ASTArrayInit&  getArrayInit();
   void                 setArrayInit(ASTArrayInit* parrayinit);

private:
   ASTExpression* mpExpression;
   ASTArrayInit*  mpArrayInit;
};

#endif // AST_VARIABLE_INIT_H
