
#ifndef AST_MEMBER_H_
#define AST_MEMBER_H_

#include "astnode.h"

class ASTModifiers;
class ASTType;

class ASTMember : public ASTNode
{
public:
   enum Kind { eConstructor, eFunction, eField, eProperty };

   explicit ASTMember(Kind kind);
   virtual ~ASTMember();

 // get/set
   Kind getKind() const;
   
private:
   Kind           mKind;
};

#endif // AST_MEMBER_H_
