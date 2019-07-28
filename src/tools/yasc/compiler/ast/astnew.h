
#ifndef AST_NEW_H_
#define AST_NEW_H_

#include "astexpressionpart.h"

class ASTExpression;
class ASTType;
class ASTFunction;

class ASTNew : public ASTExpressionPart
{
public:
   enum Kind { eObject, eArray, eInvalid };

   ASTNew();
   virtual ~ASTNew();

 // get/set
   Kind getKind() const;
   void setKind(Kind kind);

   const ASTType& getType() const;
         ASTType& getType();
   void           setType(ASTType* ptype);

   const ASTFunction& getConstructor() const;
   void               setConstructor(const ASTFunction& constructor);

   const ASTNodes& getArguments() const;
         ASTNodes& getArguments();

 // query
   int getArgumentCount() const;
   int getArrayDimension() const;

 // operations
   void addArgument(ASTExpression* pexpression);

 // visitor
   ACCEPT;

private:
   Kind     mKind;
   ASTType* mpType;

   const ASTFunction* mpConstructor;
};

#endif // AST_NEW_H_
