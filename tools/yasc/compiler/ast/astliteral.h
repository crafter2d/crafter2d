
#ifndef AST_LITERAL_H_
#define AST_LITERAL_H_

#include "astexpressionpart.h"
#include "asttype.h"

class Literal;
class VirtualLiteral;

class ASTLiteral : public ASTExpressionPart
{
public:
   ASTLiteral(const Literal& literal, ASTType::Kind kind);
   virtual ~ASTLiteral();

 // get/set
   const ASTType& getType() const;
   const Literal& getLiteral() const;
 
 // visitor
   ACCEPT;

private:
   ASTType*       mpType;
   const Literal& mLiteral;
};

#endif // AST_LITERAL_H_
