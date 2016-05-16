
#ifndef VIRTUAL_VARIANT_H_
#define VIRTUAL_VARIANT_H_

#include "core/string/string.h"
#include "core/core_base.h"

class DataStream;
class Object;

class CORE_API Variant
{
public:
            Variant();
            Variant(const Variant& that);
   explicit Variant(int value);
   explicit Variant(float value);
   explicit Variant(UChar value);
   explicit Variant(bool value);
   explicit Variant(const String& value);
   explicit Variant(Object& value);
           ~Variant();

   bool operator==(const Variant& that) const;
   bool operator!=(const Variant& that) const;
   bool operator<=(const Variant& that) const;
   bool operator<(const Variant& that) const;
   bool operator>(const Variant& that) const;
   bool operator>=(const Variant& that) const;

   const Variant& operator=(const Variant& that);

 // getters
   int asInt() const;
   void setInt(int value);

   float asReal() const;
   void setReal(float value);

   bool asBool() const;
   void setBool(bool value);

   UChar asChar() const;
   void setChar(UChar value);

   const String& asString() const;
   void setString(const String& value);

   template <class T> T& asObject() const;
   void setObject(const Object& t);

 // query
   bool isEmpty() const;
   bool isInt() const;
   bool isReal() const;
   bool isChar() const;
   bool isBool() const;
   bool isString() const;
   bool isObject() const;

 // display
   String  toString() const;
   int     toInt() const;
   float   toReal() const;
   
 // streaming
   friend CORE_API DataStream& operator<<(DataStream& stream, const Variant& variant);
   friend CORE_API DataStream& operator>>(DataStream& stream, Variant& variant);

private:
   enum MetaType
   {
      eEmpty,
      eBool,
      eInt,
      eReal,
      eChar,
      eString,
      eObject,
   };

   union Value
   {
      bool     mBoolean;
      int      mInt;
      float    mReal;
      UChar    mChar;
      String*  mpString;
      Object*  mpObject;
   };
   
   MetaType        mType;
   Value           mValue;
};


template <class T>
T& Variant::asObject() const
{
   return *static_cast<T*>(mValue.mpObject);
}

#endif // VIRTUAL_VARIANT_H_
