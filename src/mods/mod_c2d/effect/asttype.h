
#ifndef AST_TYPE_H
#define AST_TYPE_H

class ASTEffect;
class ASTStruct;
class String;

class ASTType
{
public:
   enum Type { eVoid, eFloat2, eFloat3, eFloat4, eMat2, eMat3, eMat4, eStruct, eUnknown };

   static ASTType* fromString(const ASTEffect& effect, const String& typestr);

   ASTType();
   ASTType(Type type);

 // get/set
   Type getType() const;

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
   const ASTStruct*  mpStruct;
};

#endif // AST_TYPE_H
