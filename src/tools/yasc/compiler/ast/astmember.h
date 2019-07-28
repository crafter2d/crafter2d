
#ifndef AST_MEMBER_H_
#define AST_MEMBER_H_

#include "astnode.h"

class ASTClass;
class ASTType;

class ASTMember : public ASTNode
{
public:
   enum Kind { eConstructor, eFunction, eField, eProperty };

   explicit ASTMember(Kind kind);
   virtual ~ASTMember();

 // get/set
   const ASTClass& getClass() const;
   void            setClass(ASTClass& klass);

   Kind getKind() const;
  
private:

 // data
   ASTClass* mpClass;
   Kind      mKind;
};

#endif // AST_MEMBER_H_
