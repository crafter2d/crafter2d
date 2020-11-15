
#include "literal.h"

#include "core/defines.h"

Literal::Literal():
   mValue(),
   mTableIndex(-1)
{
}

Literal::Literal(const Variant& value):
   mValue(value),
   mTableIndex(-1)
{
}

Literal::Literal(const Literal& that):
   mValue(that.mValue),
   mTableIndex(-1)
{
}

bool Literal::operator==(const Literal& that) const
{
   return mValue == that.getValue();
}

// - Get/set
   
const Variant& Literal::getValue() const
{
   return mValue;
}

void Literal::setValue(const Variant& variant)
{
   mValue = variant;
}

int Literal::getTableIndex() const
{
   return mTableIndex;
}

void Literal::setTableIndex(int index)
{
   ASSERT(index >= 0);
   mTableIndex = index;
}

// operations

Literal* Literal::clone() const
{
   return new Literal(*this);
}
