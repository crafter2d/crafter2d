
#ifndef LITERAL_TABLE_H_
#define LITERAL_TABLE_H_

#include <vector>

class Literal;
class Variant;

class LiteralTable
{
   typedef std::vector<Literal*> Literals;

public:
   LiteralTable();
   ~LiteralTable();

   const Literal& operator[](int index) const;

 // query
   bool contains(const Literal& literal) const;

   int indexOf(const Literal& literal) const;
   int indexOf(const Variant& value) const;

   int size() const;

 // maintenance
   int insert(Literal* pliteral);
   void clear();

 // search
   const Literal* find(const Variant& value) const;
         Literal* find(const Variant& value);

private:
   Literals mLiterals;
};

#endif // LITERAL_TABLE_H_
