
#include "variant.h"

#include "core/streams/datastream.h"
#include "core/conv/lexical.h"
#include "core/conv/numberconverter.h"
#include "core/defines.h"

// - Variant

Variant::Variant():
   mType(eEmpty),
   mValue()
{
}

Variant::Variant(const Variant& that):
   mType(that.mType),
   mValue()
{
   operator=(that);
}

Variant::Variant(int value):
   mType(eInt),
   mValue()
{
   mValue.mInt = value;
}

Variant::Variant(float value):
   mType(eReal),
   mValue()
{
   mValue.mReal = value;
}

Variant::Variant(UChar value):
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

Variant::Variant(const String& value):
   mType(eString),
   mValue()
{
   mValue.mpObject = value.clone();
}

Variant::~Variant()
{
   if ( mType >= eString )
   {
      delete mValue.mpObject;
   }
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
            return mValue.mpObject->equals(*that.mValue.mpObject);
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
   ASSERT(!isObject() && !isBool());
   ASSERT(mType == that.mType);

   switch ( mType )
   {
      case eInt:
         return asInt() <= that.asInt();

      case eReal:
         return asReal() <= that.asReal();

      case eChar:
         return asChar() <= that.asChar();

      case eString:
         return asString() <= that.asString();
      
      case eBool:
      case eObject:
      case eEmpty:
         UNREACHABLE("Invalid type");
   }

   return false;
}

bool Variant::operator<(const Variant& that) const
{
   ASSERT(!isObject() && !isBool());
   ASSERT(mType == that.mType);

   switch ( mType )
   {
      case eInt:
         return asInt() < that.asInt();

      case eReal:
         return asReal() < that.asReal();

      case eChar:
         return asChar() < that.asChar();

      case eString:
         return asString() < that.asString();
      
      case eBool:
      case eObject:
      case eEmpty:
         UNREACHABLE("Invalid type");
   }

   return false;
}

bool Variant::operator>(const Variant& that) const
{
   ASSERT(!isObject() && !isBool());
   ASSERT(mType == that.mType);

   switch ( mType )
   {
      case eInt:
         return asInt() > that.asInt();

      case eReal:
         return asReal() > that.asReal();

      case eChar:
         return asChar() > that.asChar();

      case eString:
         return asString() > that.asString();
      
      case eBool:
      case eObject:
      case eEmpty:
         UNREACHABLE("Invalid type");
   }

   return false;
}

bool Variant::operator>=(const Variant& that) const
{
   ASSERT(!isObject() && !isBool());
   ASSERT(mType == that.mType);

   switch ( mType )
   {
      case eInt:
         return asInt() >= that.asInt();

      case eReal:
         return asReal() >= that.asReal();

      case eChar:
         return asChar() >= that.asChar();

      case eString:
         return asString() >= that.asString();
      
      case eBool:
      case eObject:
      case eEmpty:
         UNREACHABLE("Invalid type");
   }

   return false;
}

const Variant& Variant::operator=(const Variant& that)
{
   mType = that.mType;
   if ( mType > eChar )
      mValue.mpObject = that.mValue.mpObject->clone();
   else
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

float Variant::asReal() const
{
   return mValue.mReal;
}

void Variant::setReal(float value)
{
   mType = eReal;
   mValue.mReal = value;
}

UChar Variant::asChar() const
{
   ASSERT(mType == eChar);
   return mValue.mChar;
}

void Variant::setChar(UChar value)
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

const String& Variant::asString() const
{
   return *static_cast<String*>(mValue.mpObject);
}

void Variant::setString(const String& value)
{
   mType = eString;
   mValue.mpObject = value.clone();
}

void Variant::setObject(const Object& obj)
{
   mType = eObject;
   mValue.mpObject = obj.clone();
}

// - Query

bool Variant::isEmpty() const
{
   return mType == eEmpty;
}

bool Variant::isBool() const
{
   return mType == eBool;
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

bool Variant::isString() const
{
   return mType == eString;
}

bool Variant::isObject() const
{
   return mType == eObject;
}

// - Display

String Variant::toString() const
{
   switch ( mType )
   {
      case eBool:
         return asBool() ? String("true") : String("false");
      case eInt:
         return String(lexical_cast<std::string>(asInt()).c_str());
      case eReal:
         return String(lexical_cast<std::string>(asReal()).c_str());
      case eChar:
         return String(lexical_cast<std::string>(asChar()).c_str());
      case eString:
         return asString();
      case eObject:
         return mValue.mpObject->toString();
         
      case eEmpty:
      default:
         return String::empty();
   }
}

int Variant::toInt() const
{
   switch ( mType )
   {
   case eInt:
      return asInt();
   case eReal:
      return static_cast<int>(asReal());
   case eString:
      return NumberConverter::getInstance().toInt(asString());

   default:
      UNREACHABLE("Can not convert this type to int!");
   }
}

float Variant::toReal() const
{
   switch ( mType )
   {
      case eInt:
         return static_cast<float>(asInt());
      case eReal:
         return asReal();
      
      default:
         UNREACHABLE("Can not convert type to real!");
   }
}

// - Streaming

CORE_API DataStream& operator<<(DataStream& stream, const Variant& variant)
{
   stream << (int) variant.mType;
   switch ( variant.mType )
   {
   case Variant::eBool:
      stream << variant.asBool();
      break;
   case Variant::eInt:
      stream << variant.asInt();
      break;
   case Variant::eReal:
      stream << variant.asReal();
      break;
   case Variant::eChar:
      stream << variant.asChar();
      break;
   case Variant::eString:
      stream << variant.asString();
      break;
   default:
      UNREACHABLE("not supported for streaming");
      break;
   }
   return stream;
}

CORE_API DataStream& operator>>(DataStream& stream, Variant& variant)
{
   stream >> (int&)variant.mType;
   switch ( variant.mType )
   {
   case Variant::eBool:
      stream >> variant.mValue.mBoolean;
      break;
   case Variant::eInt:
      stream >> variant.mValue.mInt;
      break;
   case Variant::eReal:
      stream >> variant.mValue.mReal;
      break;
   case Variant::eChar:
      stream >> variant.mValue.mChar;
      break;
   case Variant::eString:
      {
         String value;
         stream >> value;
         variant.setString(value);
      }
      break;
   default:
      UNREACHABLE("not supported for streaming");
      break;
   }
   return stream;
}
