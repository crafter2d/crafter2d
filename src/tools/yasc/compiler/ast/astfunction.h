
#ifndef AST_FUNCTION_H_
#define AST_FUNCTION_H_

#include "core/string/string.h"

#include "astmember.h"
#include "astmodifier.h"
#include "asttypelist.h"

class ASTAnnotations;
class ASTBlock;
class ASTClass;
class ASTLocalVariable;
class ASTFunctionArgument;
class ASTTypeVariables;
class ASTSignature;

class ASTFunction : public ASTMember
{
public:
   explicit ASTFunction(ASTMember::Kind kind);
   virtual ~ASTFunction();

 // get/set
   const String& getName() const;
   void          setName(const String& name);

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

   const ASTFunction& getBaseFunction() const;
   void               setBaseFunction(ASTFunction& function);

   bool            hasBody() const;
   const ASTBlock& getBody() const;
         ASTBlock& getBody();
   void            setBody(ASTBlock* pbody);

   const ASTSignature& getSignature() const;

   const ASTTypeList& getArguments() const;
   const ASTTypeList& getLocals() const;

   int  getLocalVariableCount() const;
   void setLocalVariableCount(int count);

 // query
   bool isConstructor() const;
   bool isDefaultConstructor() const;
   bool isGeneric() const;
   bool isVirtual() const;

   bool isTypeName(const String& name) const;

   const ASTNodes& getArgumentNodes() const;

   int getArgumentCount() const;

   String getPrototype() const;

 // operations
   void addArgument(ASTFunctionArgument* pargument);

   void addArgument(ASTType* pargument);
   void addLocal(ASTType* plocal);
   void cleanup();
   
 // visitor
   ACCEPT;

private:
   String            mName;
   ASTTypeList       mArguments;
   ASTTypeList       mLocals;
   ASTAnnotations*   mpAnnotations;
   ASTModifiers      mModifiers;
   ASTTypeVariables* mpTypeVariables;
   ASTType*          mpType;
   ASTClass*         mpClass;
   ASTFunction*      mpBaseFunction;
   ASTBlock*         mpBody;
   ASTSignature*     mpSignature;
   int               mResourceIndex;
   int               mLocalVariableCount;
};

#endif // AST_FUNCTION_H_
