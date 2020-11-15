
#ifndef AST_TYPE_H_
#define AST_TYPE_H_

#include "core/string/string.h"

#include "../antlr/antlrtextposition.h"
#include "asttypelist.h"

class ASTClass;
class ASTFunction;
class ASTTypeVariable;
class ASTTypeVariables;
class CompileContext;

class ASTType
{
public:
   enum Kind { eNull, eBoolean, eInt, eReal, eChar, eString, eVoid, eObject, eArray, eGeneric, eUnknown, eInvalid };

   static ASTType SVoidType;

   static ASTType greaterType(const ASTType& left, const ASTType& right);

   static ASTType* fromString(const String& type);

            ASTType();
   explicit ASTType(Kind kind);
            ASTType(const ASTType& that);
           ~ASTType();

   const ASTType& operator=(const ASTType& that);

 // get/set
   Kind getKind() const;
   void setKind(Kind kind);

   const String& getObjectName() const;
   void          setObjectName(const String& name);

   bool            hasObjectClass() const;
   const ASTClass& getObjectClass() const;
         ASTClass& getObjectClass();
   void            setObjectClass(ASTClass& pclass);

   int  getArrayDimension() const;
   void setArrayDimension(int dimension);

   const ASTType& getArrayType() const;
         ASTType& getArrayType();
   void           setArrayType(ASTType* ptype);

   const ASTTypeList& getTypeArguments() const;
         ASTTypeList& getTypeArguments();

   const ASTTypeVariable& getTypeVariable() const;
   void                   setTypeVariable(const ASTTypeVariable& typevariable);

   const AntlrTextPosition& getPosition() const;
   void                     setPosition(const AntlrTextPosition& pos);

 // query
   bool isValid() const;
   bool isGeneric() const;
   bool isVoid() const;
   bool isNumeric() const;
   bool isUnknown() const;
   bool isValueType() const;
   
   bool isNull() const;
   bool isArray() const;
   bool isObject() const;
   bool isBoolean() const;
   bool isInt() const;
   bool isReal() const;
   bool isChar() const;
   bool isString() const;

   bool equals(const ASTType& that) const;
   bool greater(const ASTType& that) const;

   bool isDerivedFrom(const ASTType& that) const;

 // operations
   void addTypeArgument(ASTType* ptype);
   void replaceArgument(const ASTType& type);

   bool resolveType(CompileContext& context, const ASTTypeVariables* ptypevariables);

   void clear();

   ASTType* clone() const;

 // conversion
   String toString() const;

private:

   void determineArrayDimension();

 // data
   Kind                   mKind;
   AntlrTextPosition      mPosition;
   String                 mObjectName;
   ASTClass*              mpObjectClass;
   ASTTypeList            mTypeArguments;
   const ASTTypeVariable* mpTypeVariable;
   ASTType*               mpArrayType;
   int                    mArrayDimension;
};

#endif // AST_TYPE_H_
