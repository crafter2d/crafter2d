
#ifndef LITERAL_H_
#define LITERAL_H_

#include "variant.h"

class Literal
{
public:
            Literal();
   explicit Literal(const Variant& value);
            Literal(const Literal& that);

   bool operator==(const Literal& that) const;

 // get/set
   const Variant& getValue() const;
   void           setValue(const Variant& variant);

   int  getTableIndex() const;
   void setTableIndex(int index);

 // operations
   Literal* clone() const;

private:
   Variant  mValue;
   int      mTableIndex;
};

#endif // LITERAL_H_
