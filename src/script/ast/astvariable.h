
#ifndef AST_VARIABLE_H_
#define AST_VARIABLE_H_

#include "core/string/string.h"

#include "astmodifier.h"

class ASTVariableInit;
class ASTType;

class ASTVariable
{
public:
   enum Location {
      eField,
      eArgument,
      eLocal,
      eInvalidLoc
   };

   ASTVariable();
   virtual ~ASTVariable();

 // get/set
   const String& getName() const;
   void          setName(const String& name);

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

   Location getLocation() const;
   void     setLocation(Location location);
   
   int  getResourceIndex() const;
   void setResourceIndex(int index);

 // query
   bool isArgument() const;

private:
   String            mName;
   ASTModifiers      mModifiers;
   ASTType*          mpType;
   ASTVariableInit*  mpInit;
   Location          mLocation;
   int               mResourceIndex;
};

#endif // AST_VARIABLE_H_

