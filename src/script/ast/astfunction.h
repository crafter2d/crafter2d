
#ifndef AST_FUNCTION_H_
#define AST_FUNCTION_H_

#include <string>

#include "astmember.h"
#include "astmodifier.h"

class ASTAnnotations;
class ASTBlock;
class ASTClass;
class ASTFunctionArgument;
class ASTTypeVariables;
class Signature;

class ASTFunction : public ASTMember
{
public:
   explicit ASTFunction(ASTMember::Kind kind);
   virtual ~ASTFunction();

 // get/set
   const std::string& getName() const;
   void           setName(const std::string& name);

   const ASTType& getType() const;
         ASTType& getType();
   void           setType(ASTType* ptype); 

   bool                    hasTypeVariables() const;
   const ASTTypeVariables& getTypeVariables() const;
         ASTTypeVariables& getTypeVariables();
   void                    setTypeVariables(ASTTypeVariables* ptypes);

   bool                  hasAnnotations() const;
   const ASTAnnotations& getAnnotations() const;
         ASTAnnotations& getAnnotations();
   void                  setAnnotations(ASTAnnotations* pannotations);

   const ASTModifiers& getModifiers() const;
         ASTModifiers& getModifiers();
   void                setModifiers(const ASTModifiers& modifiers);

   int  getResourceIndex() const;
   void setResourceIndex(int index);
   
   const ASTClass& getClass() const;
   void            setClass(ASTClass& owner);

   bool            hasBody() const;
   const ASTBlock& getBody() const;
         ASTBlock& getBody();
   void            setBody(ASTBlock* pbody);

   const Signature& getSignature() const;

   int  getLocalVariableCount() const;
   void setLocalVariableCount(int count);

 // query
   bool isConstructor() const;
   bool isGeneric() const;

   const ASTNodes& getArguments() const;

   int getArgumentCount() const;

 // operations
   void addArgument(ASTFunctionArgument* pargument);

 // search
   const ASTFunctionArgument& resolveArgument(const std::string& name) const;
   
 // visitor
   ACCEPT;

private:
   std::string       mName;
   ASTAnnotations*   mpAnnotations;
   ASTModifiers      mModifiers;
   ASTTypeVariables* mpTypeVariables;
   ASTType*          mpType;
   ASTClass*         mpClass;
   ASTBlock*         mpBody;
   Signature*        mpSignature;
   int               mResourceIndex;
   int               mLocalVariableCount;
};

#endif // AST_FUNCTION_H_
