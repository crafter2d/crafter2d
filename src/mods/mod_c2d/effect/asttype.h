
#ifndef AST_TYPE_H
#define AST_TYPE_H

class ASTEffect;
class ASTStruct;
class String;

class ASTType
{
public:
   enum Type { eFloat2, eFloat3, eFloat4, eMat2x2, eMat3x3, eMat4x4, eMat3x4, eStruct, eUnknown };

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
   String toString() const;

private:

 // data
   Type              mType;
   const ASTStruct*  mpStruct;
};

#endif // AST_TYPE_H
