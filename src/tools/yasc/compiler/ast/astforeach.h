
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

   const ASTVariable& getIteratorVariable() const;
         ASTVariable& getIteratorVariable();

   const ASTStatement& getBody() const;
         ASTStatement& getBody();
   void                setBody(ASTStatement* pbody);
   
 // query
   virtual bool hasReturn(bool& hasunreachablecode) const;

 // visit
   ACCEPT;

private:
 // get/set
   void setIteratorVariable(ASTVariable* pvariable);

 // operations
   void createIteratorVariable();

   ASTVariable*   mpVariable;
   ASTVariable*   mpIterator;
   ASTStatement*  mpBody;
};

#endif // AST_FOREACH_H_
