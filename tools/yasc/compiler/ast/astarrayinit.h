
#ifndef AST_ARRAY_INIT_H
#define AST_ARRAY_INIT_H

#include "astnode.h"

class ASTVariableInit;
class ASTType;

class ASTArrayInit : public ASTNode
{
public:
   ASTArrayInit();

 // get/set
   const ASTType& getType() const;

   int                    getCount() const;
   const ASTVariableInit& getInit(int index) const;

 // operations
   void determineType();

 // visitor
   ACCEPT;

private:
   ASTType* mpType;
};

#endif // AST_ARRAY_INIT_H
