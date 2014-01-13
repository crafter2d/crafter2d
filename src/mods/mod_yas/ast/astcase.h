
#ifndef AST_CASE_H_
#define AST_CASE_H_

#include "astnode.h"

#include "script/common/variant.h"

#include "asttype.h"

class ASTUnary;

class ASTCase : public ASTNode
{
public:
   enum Kind { eCase, eDefault };

   ASTCase();
   virtual ~ASTCase();

 // get/set
   void setKind(Kind kind);

   bool           hasValue() const;
   const Variant& getValue() const;
   void           setValue(const Variant& value);

   bool            hasValueExpression() const;
   const ASTUnary& getValueExpression() const;
         ASTUnary& getValueExpression();
   void            setValueExpression(ASTUnary* pvalueexpr);

   const ASTNode& getBody() const;
         ASTNode& getBody();
   void           setBody(ASTNode* pbody);

   const ASTType& getType() const;
   void           setType(ASTType* ptype);

 // query
   bool isCase() const;
   bool isDefault() const;
   
 // visitor
   ACCEPT;

private:
   Kind     mKind;
   ASTType* mpType;
   Variant  mValue;
   ASTUnary* mpValueExpression;
   ASTNode*  mpBody;
};

#endif // AST_CASE_H_
