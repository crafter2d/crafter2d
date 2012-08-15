
#ifndef SIGNATURE_H_
#define SIGNATURE_H_

#include <vector>

class ASTType;
class ASTTypeList;
class String;

class Signature
{
   typedef std::vector<const ASTType*> Types;

public:
   enum Match { eExact, eBest };

   Signature();
   ~Signature();

 // query
   const ASTType& operator[](int index) const;
   int size() const;

   bool exactMatch(const Signature& that) const;
   bool bestMatch(const Signature& that, const ASTTypeList& types) const;

 // operations
   void append(const ASTType& type);
   void append(ASTType* ptype);

 // conversion
   String toString() const;

private:

   Types mTypes;
   bool  mOwning;
};

#endif // SIGNATURE_H_
