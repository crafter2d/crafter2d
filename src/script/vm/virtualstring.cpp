
#include "virtualstring.h"

VirtualString::VirtualString():
   mString()
{
}

VirtualString::VirtualString(const String& value):
   mString(value)
{
}

VirtualString::VirtualString(const VirtualString& that):
   mString(that.mString)
{
}

// - Operators

bool VirtualString::operator==(const VirtualString& that) const
{
   return mString == that.mString;
}

bool VirtualString::operator<=(const VirtualString& that) const
{
   return mString <= that.mString;
}

bool VirtualString::operator<(const VirtualString& that) const
{
   return mString < that.mString;
}

bool VirtualString::operator>(const VirtualString& that) const
{
   return mString > that.mString;
}

bool VirtualString::operator>=(const VirtualString& that) const
{
   return mString >= that.mString;
}

// - Get/set
   
const String& VirtualString::getString() const
{
   return mString;
}

void VirtualString::setString(const String& value)
{
   mString = value;
}

// - Overloads

void VirtualString::finalize(VirtualMachine& machine)
{
}

void VirtualString::doMark()
{
}
