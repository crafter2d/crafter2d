
#ifndef AST_VARIABLE_H_
#define AST_VARIABLE_H_

#include <string>

#include "astmodifier.h"

class ASTVariableInit;
class ASTType;

class ASTVariable
{
public:
   ASTVariable();
   virtual ~ASTVariable();

 // get/set
   const std::string& getName() const;
   void               setName(const std::string& name);

   const ASTType& getType() const;
         ASTType& getType();
   void           setType(ASTType* ptype); 

   const ASTModifiers& getModifiers() const;
         ASTModifiers& getModifiers();
   void                setModifiers(const ASTModifiers& modifiers);

   bool                   hasInit() const;
   const ASTVariableInit& getInit() const;
         ASTVariableInit& getInit();
   void                   setInit(ASTVariableInit* pinit);
   
   int  getResourceIndex() const;
   void setResourceIndex(int index);

private:
   std::string       mName;
   ASTModifiers      mModifiers;
   ASTType*          mpType;
   ASTVariableInit*  mpInit;
   int               mResourceIndex;
};

#endif // AST_VARIABLE_H_

