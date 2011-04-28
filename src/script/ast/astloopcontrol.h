
#ifndef AST_LOOP_CONTROL_H_
#define AST_LOOP_CONTROL_H_

#include "aststatement.h"

class ASTLoopControl : public ASTStatement
{
public:
   enum Kind { eBreak, eContinue, eInvalid };

   explicit ASTLoopControl(Kind kind = eInvalid);

 // get/set
   Kind getKind() const;
   void setKind(Kind kind);

 // visit
   ACCEPT;

private:
   Kind mKind;
};

#endif // AST_LOOP_CONTROL_H_
