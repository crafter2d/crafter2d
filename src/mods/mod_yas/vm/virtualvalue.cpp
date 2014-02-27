
#include "virtualvalue.h"
#ifndef CRAFTER_INLINE
#  include "virtualvalue.inl"
#endif

#include "core/conv/lexical.h"
#include "core/system/variant.h"

#include "mod_yas/common/stringcache.h"

#include "virtualobject.h"
#include "virtualclass.h"
#include "virtualstring.h"

VirtualValue::VirtualValue():
   mKind(eEmpty),
   mValue()
{
}
 
VirtualValue::VirtualValue(const VirtualValue& that):
   mKind(that.mKind),
   mValue(that.mValue)
{
}

VirtualValue::VirtualValue(bool value):
   mKind(eBool),
   mValue()
{
   mValue.boolean = value;
}

VirtualValue::VirtualValue(int value):
   mKind(eNumber),
   mValue()
{
   mValue.number = value;
}

VirtualValue::VirtualValue(double value):
   mKind(eReal),
   mValue()
{
   mValue.real = value;
}

VirtualValue::VirtualValue(UChar value):
   mKind(eChar),
   mValue()
{
   mValue.uchar = value;
}

VirtualValue::VirtualValue(VirtualString& value):
   mKind(eString),
   mValue()
{
   mValue.string = &value;
}

VirtualValue::VirtualValue(VirtualObject& object):
   mKind(eObject),
   mValue()
{
   mValue.object = &object;
}

VirtualValue::VirtualValue(VirtualArray& value):
   mKind(eArray),
   mValue()
{
   mValue.array = &value;
}

VirtualValue& VirtualValue::operator=(const VirtualValue& that)
{
   mKind = that.mKind;
   mValue = that.mValue;
   return *this;
}

// - Comparison

bool VirtualValue::operator==(const VirtualValue& that) const
{
   if ( mKind == that.mKind )
   {
      switch ( mKind )
      {
         case eEmpty:
            return true;

         case eNumber:
            return asNumber() == that.asNumber();

         case eReal:
            return asReal() == that.asReal();

         case eChar:
            return asChar() == that.asChar();

         case eBool:
            return asBoolean() == that.asBoolean();

         case eString:
            return asString() == that.asString();

         case eObject:
            return &asObject() == &that.asObject();

         case eArray:
            return &asArray() == &that.asArray();
      }
   }

   return false;
}

bool VirtualValue::operator!=(const VirtualValue& that) const
{
   return !operator==(that);
}

bool VirtualValue::operator<=(const VirtualValue& that) const
{
   ASSERT(!isArray() && !isObject() && !isBoolean());
   ASSERT(mKind == that.mKind);

   switch ( mKind )
   {
      case eNumber:
         return asNumber() <= that.asNumber();

      case eReal:
         return asReal() <= that.asReal();

      case eChar:
         return asChar() <= that.asChar();

      case eString:
         return asString() <= that.asString();
   }

   return false;
}

bool VirtualValue::operator<(const VirtualValue& that) const
{
   ASSERT(!isArray() && !isObject() && !isBoolean());
   ASSERT(mKind == that.mKind);

   switch ( mKind )
   {
      case eNumber:
         return asNumber() < that.asNumber();

      case eReal:
         return asReal() < that.asReal();

      case eChar:
         return asChar() < that.asChar();

      case eString:
         return asString() < that.asString();
   }

   return false;
}

bool VirtualValue::operator>(const VirtualValue& that) const
{
   ASSERT(!isArray() && !isObject() && !isBoolean());
   ASSERT(mKind == that.mKind);

   switch ( mKind )
   {
      case eNumber:
         return asNumber() > that.asNumber();

      case eReal:
         return asReal() > that.asReal();

      case eChar:
         return asChar() > that.asChar();

      case eString:
         return asString() > that.asString();
   }

   return false;
}

bool VirtualValue::operator>=(const VirtualValue& that) const
{
   ASSERT(!isArray() && !isObject() && !isBoolean());
   ASSERT(mKind == that.mKind);

   switch ( mKind )
   {
      case eNumber:
         return asNumber() >= that.asNumber();

      case eReal:
         return asReal() >= that.asReal();

      case eChar:
         return asChar() >= that.asChar();

      case eString:
         return asString() >= that.asString();
   }

   return false;
}

// - Get/set

const String& VirtualValue::asString() const
{
   return mValue.string->getString();
}

// - Display

String VirtualValue::typeAsString() const
{
   switch ( mKind )
   {
      case eBool:
         return String("boolean");
      case eNumber:
         return String("int");
      case eReal:
         return String("real");
      case eChar:
         return String("char");
      case eString:
         return String("string");
      case eObject:
         return mValue.object->getClass().getName();
      case eArray:
         return String("array");
   }
   return String();
}

String VirtualValue::toString() const
{
   switch ( mKind )
   {
      case eBool:
         return mValue.boolean ? String("true") : String("false");
      case eNumber:
         return String(lexical_cast<std::string>(mValue.number).c_str());
      case eReal:
         return String(lexical_cast<std::string>(mValue.real).c_str());
      case eChar:
         return String(lexical_cast<std::string>(mValue.uchar).c_str());
      case eString:
         return mValue.string->getString();
      case eObject:
         return String(mValue.object->getClass().getName());
      case eArray:
         return String("array");
   }

   return String();
}

// - Conversion

void VirtualValue::int2real()
{
   ASSERT(isNumber());
   setReal((double)asNumber());
}

void VirtualValue::real2int()
{
   ASSERT(isReal());
   setNumber((int)asReal());
}

void VirtualValue::setTo(const Variant& value, StringCache& stringcache)
{
   if ( value.isInt() )
   {
      setNumber(value.asInt());
   }
   else if ( value.isReal() )
   {
      setReal(value.asReal());
   }
   else if ( value.isBool() )
   {
      setBoolean(value.asBool());
   }
   else if ( value.isChar() )
   {
      setChar(value.asChar());
   }
   else if ( value.isString() )
   {
      setString(stringcache.lookup(value.asString()));
   }
   else
   {
      mKind = eEmpty;
   }
}
