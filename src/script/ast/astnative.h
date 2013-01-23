
#ifndef AST_NATIVE_H
#define AST_NATIVE_H

#include "astexpressionpart.h"

class ASTNative : public ASTExpressionPart
{
public:
   ASTNative();

 // get/set
   bool            hasArguments() const;
   const ASTNodes& getArguments() const;
         ASTNodes& getArguments();

   int  getIndex() const;
   void setIndex(int index);

 // visitor
   ACCEPT;

private:
   int mIndex;
};

#endif // AST_NATIVE_H
