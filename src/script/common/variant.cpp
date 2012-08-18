
#include "variant.h"

#include "core/defines.h"
#include "core/conv/lexical.h"
#include "core/conv/numberconverter.h"

#include "script/vm/virtualarray.h"
#include "script/vm/virtualobject.h"
#include "script/vm/virtualclass.h"
#include "script/vm/virtualstring.h"

// - Variant

Variant::Variant():
   mType(eEmpty),
   mValue()
{
}

Variant::Variant(const Variant& that):
   mType(that.mType),
   mValue(that.mValue)
{
}

Variant::Variant(int value):
   mType(eInt),
   mValue()
{
   mValue.mInt = value;
}

Variant::Variant(double value):
   mType(eReal),
   mValue()
{
   mValue.mReal = value;
}

Variant::Variant(char value):
   mType(eChar),
   mValue()
{
   mValue.mChar = value;
}

Variant::Variant(bool value):
   mType(eBool),
   mValue()
{
   mValue.mBoolean = value;
}

Variant::Variant(VirtualString& value):
   mType(eString),
   mValue()
{
   mValue.mpString = &value;
}

Variant::Variant(VirtualObject& object):
   mType(eObject),
   mValue()
{
   mValue.mpObject = &object;
}

Variant::Variant(VirtualObject* pvirtualobject):
   mType(eObject),
   mValue()
{
   mValue.mpObject = pvirtualobject;
}

Variant::Variant(VirtualArray& array):
   mType(eArray),
   mValue()
{
   mValue.mpArray = &array;
}

Variant::~Variant()
{
}

bool Variant::operator==(const Variant& that) const
{
   if ( mType == that.mType )
   {
      switch ( mType )
      {
         case eEmpty:
            return true;

         case eInt:
            return asInt() == that.asInt();

         case eReal:
            return asReal() == that.asReal();

         case eChar:
            return asChar() == that.asChar();

         case eBool:
            return asBool() == that.asBool();

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

bool Variant::operator!=(const Variant& that) const
{
   return !operator==(that);
}

bool Variant::operator<=(const Variant& that) const
{
   ASSERT(!isArray() && !isObject() && !isBool());
   ASSERT(mType == that.mType);

   switch ( mType )
   {
      case eInt:
         return asInt() <= that.asInt();

      case eReal:
         return asReal() <= that.asReal();

      case eString:
         return asString() <= that.asString();
   }

   return false;
}

bool Variant::operator<(const Variant& that) const
{
   ASSERT(!isArray() && !isObject() && !isBool());
   ASSERT(mType == that.mType);

   switch ( mType )
   {
      case eInt:
         return asInt() < that.asInt();

      case eReal:
         return asReal() < that.asReal();

      case eString:
         return asString() < that.asString();
   }

   return false;
}

bool Variant::operator>(const Variant& that) const
{
   ASSERT(!isArray() && !isObject() && !isBool());
   ASSERT(mType == that.mType);

   switch ( mType )
   {
      case eInt:
         return asInt() > that.asInt();

      case eReal:
         return asReal() > that.asReal();

      case eString:
         return asString() > that.asString();
   }

   return false;
}

bool Variant::operator>=(const Variant& that) const
{
   ASSERT(!isArray() && !isObject() && !isBool());
   ASSERT(mType == that.mType);

   switch ( mType )
   {
      case eInt:
         return asInt() >= that.asInt();

      case eReal:
         return asReal() >= that.asReal();

      case eString:
         return asString() >= that.asString();
   }

   return false;
}

const Variant& Variant::operator=(const Variant& that)
{
   mType = that.mType;
   mValue = that.mValue;

   return *this;
}

// - Getters

int Variant::asInt() const
{
   return mValue.mInt;
}

void Variant::setInt(int value)
{
   mType = eInt;
   mValue.mInt = value;
}

double Variant::asReal() const
{
   return mValue.mReal;
}

void Variant::setReal(double value)
{
   mType = eReal;
   mValue.mReal = value;
}

char Variant::asChar() const
{
   ASSERT(mType == eChar);
   return mValue.mChar;
}

void Variant::setChar(char value)
{
   mType = eChar;
   mValue.mChar = value;
}

bool Variant::asBool() const
{
   return mValue.mBoolean;
}

void Variant::setBool(bool value)
{
   mType = eBool;
   mValue.mBoolean = value;
}

VirtualString& Variant::asString() const
{
   return *mValue.mpString;
}

void Variant::setString(VirtualString& value)
{
   mType = eString;
   mValue.mpString = &value;
}

VirtualObject& Variant::asObject() const
{
   return *mValue.mpObject;
}

void Variant::setObject(VirtualObject& object)
{
   mType = eObject;
   mValue.mpObject = &object;
}

VirtualArray& Variant::asArray() const
{
   return *mValue.mpArray;
}

void Variant::setArray(VirtualArray& array)
{
   mType = eArray;
   mValue.mpArray = &array;
}

// - Query

bool Variant::isEmpty() const
{
   return mType == eEmpty;
}

bool Variant::isInt() const
{
   return mType == eInt;
}

bool Variant::isReal() const
{
   return mType == eReal;
}

bool Variant::isChar() const
{
   return mType == eChar;
}

bool Variant::isBool() const
{
   return mType == eBool;
}

bool Variant::isString() const
{
   return mType == eString;
}

bool Variant::isObject() const
{
   return mType == eObject;
}

bool Variant::isArray() const
{
   return mType == eArray;
}

// - Display

String Variant::typeAsString() const
{
   switch ( mType )
   {
      case eString:
         return String("string");
      case eInt:
         return String("int");
      case eReal:
         return String("real");
      case eChar:
         return String("char");
      case eBool:
         return String("boolean");
      case eObject:
         return String(asObject().getClass().getName());
      case eArray:
         return String("array");
   }
   return String();
}

String Variant::toString() const
{
   switch ( mType )
   {
      case eString:
         return asString().getString();
      case eInt:
         return String(lexical_cast<std::string>(asInt()).c_str());
      case eReal:
         return String(lexical_cast<std::string>(asReal()).c_str());
      case eChar:
         return String(lexical_cast<std::string>(asChar()).c_str());
      case eBool:
         return asBool() ? String("true") : String("false");
      case eObject:
         return String(asObject().getClass().getName());
      case eArray:
         return String("array");
   }

   return String();
}

int Variant::toInt() const
{
   switch ( mType )
   {
   case eInt:
      break;
   case eReal:
      return static_cast<int>(asReal());
   case eString:
      return NumberConverter::getInstance().toInt(asString().getString());

   default:
      UNREACHABLE("Can not convert this type to int!");
   }

   return asInt(); // currently no other int types
}

double Variant::toReal() const
{
   switch ( mType )
   {
      case eInt:
         return asInt();
      case eReal:
         return asReal();
   }

   return 0.0;
}

// - Conversion

void Variant::int2real()
{
   ASSERT(isInt());
   setReal((double)asInt());
}

void Variant::real2int()
{
   ASSERT(isReal());
   setInt((int)asReal());
}

