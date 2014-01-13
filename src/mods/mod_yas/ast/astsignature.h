
#ifndef AST_SIGNATURE_H_
#define AST_SIGNATURE_H_

#include <vector>

class ASTType;
class ASTTypeList;
class String;

class ASTSignature
{
   typedef std::vector<const ASTType*> Types;

public:
   enum Match { eExact, eBest };

   ASTSignature();
   ~ASTSignature();

 // query
   const ASTType& operator[](int index) const;
   int size() const;

   bool exactMatch(const ASTSignature& that) const;
   bool bestMatch(const ASTSignature& that, const ASTTypeList& types) const;

 // operations
   void append(const ASTType& type);
   void append(ASTType* ptype);

 // conversion
   String toString() const;

private:

   Types mTypes;
   bool  mOwning;
};

#endif // AST_SIGNATURE_H_
