
#include "Variant.h"

#include "core/defines.h"
#include "core/conv/lexical.h"

#include "script/vm/virtualobjectreference.h"
#include "script/vm/virtualarrayreference.h"

// - Variant

Variant::Variant():
   mType(eEmpty),
   mpHolder(NULL)
{
}

Variant::Variant(const Variant& that):
   mType(that.mType),
   mpHolder(isEmpty() ? NULL : that.mpHolder->clone())
{
}

Variant::Variant(int value):
   mType(eInt),
   mpHolder(new DataHolder<int>(value))
{
}

Variant::Variant(double value):
   mType(eReal),
   mpHolder(new DataHolder<double>(value))
{
}

Variant::Variant(bool value):
   mType(eBool),
   mpHolder(new DataHolder<bool>(value))
{
}

Variant::Variant(const std::string& value):
   mType(eString),
   mpHolder(new DataHolder<std::string>(value))
{
}

Variant::Variant(const VirtualObjectReference& object):
   mType(eObject),
   mpHolder(new DataHolder<VirtualObjectReference>(object))
{
}

Variant::Variant(const VirtualArrayReference& array):
   mType(eArray),
   mpHolder(new DataHolder<VirtualArrayReference>(array))
{
}

Variant::~Variant()
{
   delete mpHolder;
   mpHolder = NULL;
}

bool Variant::operator==(const Variant& that) const
{
   if ( mType == that.mType )
   {
      switch ( mType )
      {
         case eInt:
            return asInt() == that.asInt();

         case eReal:
            return asReal() == that.asReal();

         case eBool:
            return asBool() == that.asBool();

         case eString:
            return asString() == that.asString();

         case eObject:
            return asObject().ptr() == that.asObject().ptr();

         case eArray:
            return asArray().ptr() == that.asArray().ptr();
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
         return asString().compare(that.asString()) <= 0;
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
         return asString().compare(that.asString()) <= 0;
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
         return asString().compare(that.asString()) > 0;
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
         return asString().compare(that.asString()) >= 0;
   }

   return false;
}

const Variant& Variant::operator=(const Variant& that)
{
   delete mpHolder;

   mType = that.mType;
   mpHolder = (mType == eEmpty) ? NULL : that.mpHolder->clone();

   return *this;
}

// - Getters

int Variant::asInt() const
{
   return ((DataHolder<int>*)mpHolder)->mData;
}

void Variant::setInt(int value)
{
   mType = eInt;
   delete mpHolder;
   mpHolder = new DataHolder<int>(value);
}

double Variant::asReal() const
{
   return ((DataHolder<double>*)mpHolder)->mData;
}

void Variant::setReal(double value)
{
   mType = eReal;
   delete mpHolder;
   mpHolder = new DataHolder<double>(value);
}

bool Variant::asBool() const
{
   return ((DataHolder<bool>*)mpHolder)->mData;
}

void Variant::setBool(bool value)
{
   mType = eBool;
   delete mpHolder;
   mpHolder = new DataHolder<bool>(value);
}

const std::string& Variant::asString() const
{
   return ((DataHolder<std::string>*)mpHolder)->mData;
}

void Variant::setString(const std::string& value)
{
   mType = eString;
   delete mpHolder;
   mpHolder = new DataHolder<std::string>(value);
}

VirtualObjectReference& Variant::asObject() const
{
   return ((DataHolder<VirtualObjectReference>*)mpHolder)->mData;
}

void Variant::setObject(const VirtualObjectReference& object)
{
   mType = eObject;
   delete mpHolder;
   mpHolder = new DataHolder<VirtualObjectReference>(object);
}

VirtualArrayReference& Variant::asArray() const
{
   return ((DataHolder<VirtualArrayReference>*)mpHolder)->mData;
}

void Variant::setArray(const VirtualArrayReference& array)
{
   mType = eArray;
   delete mpHolder;
   mpHolder = new DataHolder<VirtualArrayReference>(array);
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

std::string Variant::toString() const
{
   switch ( mType )
   {
      case eString:
         return asString();
      case eInt:
         return lexical_cast<std::string>(asInt());
      case eReal:
         return lexical_cast<std::string>(asReal());
      case eBool:
         return asBool() ? std::string("true") : std::string("false");
      case eArray:
         return "array";
   }

   return "";
}

int Variant::toInt() const
{
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

void Variant::int2string()
{
   ASSERT(isInt());
   setString(lexical_cast<std::string>(asInt()));
}

void Variant::real2string()
{
   ASSERT(isReal());
   setString(lexical_cast<std::string>(asReal()));
}
