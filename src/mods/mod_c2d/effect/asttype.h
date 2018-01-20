
#ifndef AST_TYPE_H
#define AST_TYPE_H

#include "core/string/string.h"

class ASTEffect;
class ASTStruct;

class ASTType
{
public:
   enum Type { eVoid, eFloat, eFloat2, eFloat3, eFloat4, eUint, eMat2, eMat3, eMat4, eStruct, eUnknown };

   static ASTType* fromString(const ASTEffect& effect, const String& typestr);

   ASTType();
   ASTType(Type type);

 // get/set
   Type getType() const;

   const String& getTemplateClass() const;
   void          setTemplateClass(const String tmplclass);

   const ASTStruct& getStruct() const;

 // query
   bool isStruct() const;
   bool isUnknown() const;

 // conversion
   String toDirectX() const;
   String toOpenGL() const;

private:

 // data
   Type              mType;
   String            mTemplateClass;
   const ASTStruct*  mpStruct;
};

#endif // AST_TYPE_H
