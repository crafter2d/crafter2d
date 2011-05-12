
#ifndef AST_TYPE_H_
#define AST_TYPE_H_

#include <string>

#include "asttypelist.h"

class ASTClass;
class ASTTypeVariable;
class CompileContext;

class ASTType
{
public:
   enum Kind { eBoolean, eInt, eReal, eString, eVoid, eObject, eArray, eGeneric, eInvalid };

   static ASTType SVoidType;

   static ASTType greaterType(const ASTType& left, const ASTType& right);

            ASTType();
   explicit ASTType(Kind kind);
            ASTType(const ASTType& that);
           ~ASTType();

   const ASTType& operator=(const ASTType& that);

 // get/set
   Kind getKind() const;
   void setKind(Kind kind);

   const std::string& getObjectName() const;
   void           setObjectName(const std::string& name);

   bool            hasObjectClass() const;
   const ASTClass& getObjectClass() const;
         ASTClass& getObjectClass();
   void            setObjectClass(ASTClass& pclass);

   int  getArrayDimension() const;
   void setArrayDimension(int dimension);

   const ASTType& getArrayType() const;
   void           setArrayType(ASTType* ptype);

   const ASTTypeList& getTypeArguments() const;

   const ASTTypeVariable& getTypeVariable() const;
   void                   setTypeVariable(const ASTTypeVariable& typevariable);

   int  getLine() const;
   void setLine(int line);

   int  getPos() const;
   void setPos(int pos);

 // query
   bool isValid() const;
   bool isGeneric() const;
   bool isVoid() const;
   bool isNumeric() const;

   bool isArray() const;
   bool isObject() const;
   bool isBoolean() const;
   bool isInt() const;
   bool isReal() const;
   bool isString() const;

   bool equals(const ASTType& that) const;
   bool greater(const ASTType& that) const;

 // operations
   void addTypeArgument(ASTType* ptype);
   void replaceArgument(const ASTType& type);

   bool resolveType(CompileContext& context, const ASTClass& aclass);

   void clear();

   ASTType* clone() const;

 // conversion
   std::string toString() const;

private:

   Kind                   mKind;
   std::string            mObjectName;
   ASTClass*              mpObjectClass;
   ASTTypeList            mTypeArguments;
   const ASTTypeVariable* mpTypeVariable;
   ASTType*               mpArrayType;
   int                    mArrayDimension;
   int                    mLine;
   int                    mPos;
};

#endif // AST_TYPE_H_
