
#ifndef AST_SUPER_H_
#define AST_SUPER_H_

#include "astexpressionpart.h"

class ASTFunction;

class ASTSuper : public ASTExpressionPart
{
public:
   enum Kind { eThis, eSuper };

   ASTSuper();

 // get/set
   bool               hasConstructor() const;
   const ASTFunction& getConstructor() const;
   void               setConstructor(ASTFunction* pmember);

   Kind getKind() const;
   void setKind(Kind kind);

   bool isCall() const;
   void setCall(bool call);

 // query
   int       getArgumentCount() const;
   ASTNodes& getArguments();

   bool isSuper() const;
   bool isThis() const;

 // visitor
   ACCEPT;

private:
   ASTFunction* mpConstructor;
   Kind         mKind;
   bool         mCall;
};

#endif // AST_SUPER_H_
