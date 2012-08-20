
#ifndef VIRTUAL_VARIANT_H_
#define VIRTUAL_VARIANT_H_

#include "script/script_base.h"

#include "core/string/string.h"

class VirtualObject;
class VirtualArray;
class VirtualString;

class SCRIPT_API Variant
{
public:
            Variant();
            Variant(const Variant& that);
   explicit Variant(int value);
   explicit Variant(double value);
   explicit Variant(char value);
   explicit Variant(bool value);
   explicit Variant(VirtualString& value);
   explicit Variant(VirtualObject& object);
   explicit Variant(VirtualObject* pvirtualobject);
   explicit Variant(VirtualArray& array);
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

   double asReal() const;
   void setReal(double value);

   bool asBool() const;
   void setBool(bool value);

   char asChar() const;
   void setChar(char value);

   VirtualString& asString() const;
   void setString(VirtualString& value);

   VirtualObject& asObject() const;
   void setObject(VirtualObject& object);

   VirtualArray& asArray() const;
   void setArray(VirtualArray& array);

 // query
   bool isEmpty() const;
   bool isInt() const;
   bool isReal() const;
   bool isChar() const;
   bool isBool() const;
   bool isString() const;
   bool isObject() const;
   bool isArray() const;

 // display
   String  typeAsString() const;
   String  toString() const;
   int     toInt() const;
   double  toReal() const;

 // conversion
   void int2real();
   void real2int();

private:
   enum MetaType
   {
      eEmpty,
      eInt,
      eReal,
      eString,
      eChar,
      eBool,
      eObject,
      eArray
   };

   union Value
   {
      int            mInt;
      double         mReal;
      char           mChar;
      bool           mBoolean;
      VirtualString* mpString;
      VirtualObject* mpObject;
      VirtualArray*  mpArray;
   };
   
   MetaType        mType;
   Value           mValue;
};

#endif // VIRTUAL_VARIANT_H_
