
#ifndef AST_ACCESS_H_
#define AST_ACCESS_H_

#include "core/string/string.h"

#include "astexpressionpart.h"
#include "asttypelist.h"

class ASTExpression;
class ASTFunction;
class ASTVariable;
class ASTType;

class ASTAccess : public ASTExpressionPart
{
public:
   enum Kind { eVariable, eFunction, eArray, eStatic, eClass, eInvalid };

   enum VarAccess { 
      eField,           // access field, pushes the class(name) first on stack
      eRefField,        // field of topmost object on stack
      eArgument,        // argument of the function, location is on stack
      eLocal,           // local variable, space is on stack
      eArrayAccess,     // array access, the array object should be on the stack already
      eInvalidAccess
   };

   ASTAccess();
   virtual ~ASTAccess();

 // get/set
   const String& getName() const;
   void          setName(const String& name);

   Kind getKind() const;
   void setKind(Kind kind);

   VarAccess getAccess() const;
   void      setAccess(VarAccess access);

   bool       hasNext() const;
   ASTAccess& getNext();
   void       setNext(ASTAccess& access);

   bool       hasPrevious() const;
   ASTAccess& getPrevious();
   void       setPrevious(ASTAccess& access);

   const ASTNodes& getArguments() const;
         ASTNodes& getArguments();

   const ASTFunction& getFunction() const;
   void               setFunction(const ASTFunction& member);

   const ASTVariable& getVariable() const;
   void               setVariable(const ASTVariable& var);

   const ASTType& getStaticType() const;
   void           setStaticType(ASTType* ptype);

   const ASTTypeList& getTypeArguments() const;
         ASTTypeList& getTypeArguments();

 // operations
   void addArgument(ASTExpression* pexpression);
   void replaceArgument(int index, ASTNode* pnode);

 // visitor
   ACCEPT;

private:
   String mName;
   Kind        mKind;
   VarAccess   mVarAccess;

   ASTAccess* mpNext;
   ASTAccess* mpPrevious;

   ASTTypeList mTypeArguments;

   const ASTFunction* mpFunction;
   const ASTVariable* mpVariable;
         ASTType*     mpStaticType;
};

#endif // AST_ACCESS_H_
