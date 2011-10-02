
#ifndef AST_INSTANCEOF_H_
#define AST_INSTANCEOF_H_

#include "astnode.h"

class ASTType;

class ASTInstanceOf : public ASTNode
{
public:
   ASTInstanceOf();
   virtual ~ASTInstanceOf();

 // get/set
   const ASTType& getInstanceType() const;
         ASTType& getInstanceType();
   void           setInstanceType(ASTType* ptype);

   const ASTNode& getObject() const;
         ASTNode& getObject();
   void           setObject(ASTNode* pobject);

 // visiting
   ACCEPT;

private:
   ASTType* mpInstanceType;
   ASTNode* mpObject;
};

#endif // AST_INSTANCEOF_H_
