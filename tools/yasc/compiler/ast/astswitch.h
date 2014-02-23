
#ifndef AST_SWITCH_H_
#define AST_SWITCH_H_

#include "aststatement.h"

class CompileContext;
class ASTCase;
class ASTType;

class ASTSwitch : public ASTStatement
{
public:
   ASTSwitch();
   virtual ~ASTSwitch();

 // get/set
   const ASTNode& getExpression() const;
         ASTNode& getExpression();
   void           setExpression(ASTNode* pexpression);

   const ASTCase& getCase(int index) const;

   const ASTType& getType() const;
   void           setType(ASTType* ptype);

 // query
   int getDefaultCount() const;
   int getTotalCount() const;

   bool hasDefault() const;
   bool canLookup() const;

   virtual bool hasReturn(bool& hasunreachablecode) const;

 // operations
   void validateCaseTypes(CompileContext& context);

 // visitor
   ACCEPT;

private:
   ASTNode* mpExpression;
   ASTType* mpType;
};

#endif // AST_SWITCH_H_
