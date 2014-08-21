
#include "core/defines.h"

INLINE bool VirtualValue::isEmpty() const
{
   return mKind == eEmpty;
}

INLINE bool VirtualValue::isBoolean() const
{
   return mKind == eBool;
}

INLINE bool VirtualValue::isNumber() const
{
   return mKind == eNumber;
}

INLINE bool VirtualValue::isReal() const
{
   return mKind == eReal;
}

INLINE bool VirtualValue::isChar() const
{
   return mKind == eChar;
}

INLINE bool VirtualValue::isString() const
{
   return mKind == eString;
}

INLINE bool VirtualValue::isObject() const
{
   return mKind == eObject;
}

INLINE bool VirtualValue::isArray() const
{
   return mKind == eArray;
}

INLINE bool VirtualValue::asBoolean() const
{
   return mValue.boolean;
}

INLINE int VirtualValue::asNumber() const
{
   return mValue.number;
}

INLINE float VirtualValue::asReal() const
{
   return mValue.real;
}

INLINE UChar VirtualValue::asChar() const
{
   return mValue.uchar;
}

INLINE VirtualObject& VirtualValue::asObject() const
{
   return *mValue.object;
}

INLINE VirtualArray& VirtualValue::asArray() const
{
   return *mValue.array;
}

INLINE void VirtualValue::setBoolean(bool value)
{
   mKind = eBool;
   mValue.boolean = value;
}

INLINE void VirtualValue::setNumber(int value)
{
   mKind = eNumber;
   mValue.number = value;
}

INLINE void VirtualValue::setReal(float value)
{
   mKind = eReal;
   mValue.real = value;
}

INLINE void VirtualValue::setChar(UChar value)
{
   mKind = eChar;
   mValue.uchar = value;
}

INLINE void VirtualValue::setString(VirtualString& value)
{
   mKind = eString;
   mValue.string = &value;
}

INLINE void VirtualValue::setObject(VirtualObject& value)
{
   mKind = eObject;
   mValue.object = &value;
}

INLINE void VirtualValue::setArray(VirtualArray& value)
{
   mKind = eArray;
   mValue.array = &value;
}
