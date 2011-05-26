
#ifndef AST_CASE_H_
#define AST_CASE_H_

#include "astnode.h"

#include "script/common/variant.h"

#include "asttype.h"

class ASTCase : public ASTNode
{
public:
   enum Kind { eCase, eDefault };

   ASTCase();
   virtual ~ASTCase();

 // get/set
   void setKind(Kind kind);

   const Variant& getValue() const;
   void           setValue(const Variant& value);

   const ASTNode& getBody() const;
         ASTNode& getBody();
   void           setBody(ASTNode* pbody);

   const ASTType& getType() const;

 // query
   bool isCase() const;
   bool isDefault() const;
   
 // visitor
   ACCEPT;

private:
 // get/set
   void setType(ASTType* ptype);

   Kind     mKind;
   ASTType* mpType;
   ASTNode* mpBody;
   Variant  mValue;
};

#endif // AST_CASE_H_
