
#ifndef AST_FOREACH_H_ 
#define AST_FOREACH_H_

#include "aststatement.h"

class ASTVariable;

class ASTForeach : public ASTStatement
{
public:
   ASTForeach();
   virtual ~ASTForeach();

 // get/set
   const ASTVariable& getVariable() const;
         ASTVariable& getVariable();
   void               setVariable(ASTVariable* pvariable);

   const ASTStatement& getBody() const;
         ASTStatement& getBody();
   void                setBody(ASTStatement* pbody);

   int  getResourceIndex() const;
   void setResourceIndex(int index);

 // query
   virtual bool hasReturn(bool& hasunreachablecode) const;

 // visit
   ACCEPT;

private:
   ASTVariable*   mpVariable;
   ASTStatement*  mpBody;
   int            mResourceIndex;
};

#endif // AST_FOREACH_H_
