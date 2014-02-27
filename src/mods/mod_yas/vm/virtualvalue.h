
#ifndef VIRTUAL_VALUE_H
#define VIRTUAL_VALUE_H

#include "core/string/string.h"

class StringCache;
class VirtualString;
class VirtualObject;
class VirtualArray;
class Variant;

class VirtualValue
{
public:
   VirtualValue();
   VirtualValue(const VirtualValue& that);
   VirtualValue(bool value);
   VirtualValue(int value);
   VirtualValue(double value);
   VirtualValue(UChar value);
   VirtualValue(VirtualString& value);
   VirtualValue(VirtualObject& object);
   VirtualValue(VirtualArray& array);
   VirtualValue& operator=(const VirtualValue& that);

 // comparison
   bool operator==(const VirtualValue& that) const;
   bool operator!=(const VirtualValue& that) const;
   bool operator<=(const VirtualValue& that) const;
   bool operator<(const VirtualValue& that) const;
   bool operator>(const VirtualValue& that) const;
   bool operator>=(const VirtualValue& that) const;

 // get/set
   bool isEmpty() const;

   bool isBoolean() const;
   bool asBoolean() const;
   void setBoolean(bool value);

   bool isNumber() const;
   int  asNumber() const;
   void setNumber(int value);

   bool   isReal() const;
   double asReal() const;
   void   setReal(double value);

   bool  isChar() const;
   UChar asChar() const;
   void  setChar(UChar value);

   bool          isString() const;
   const String& asString() const;
   void          setString(VirtualString& value);

   bool           isObject() const;
   VirtualObject& asObject() const;
   void           setObject(VirtualObject& value);

   bool          isArray() const;
   VirtualArray& asArray() const;
   void          setArray(VirtualArray& value);

 // display
   String typeAsString() const;
   String toString() const;

 // conversion
   void int2real();
   void real2int();

   void setTo(const Variant& variant, StringCache& stringcache);

private:

   enum { eBool, eNumber, eReal, eChar, eString, eObject, eArray, eEmpty } mKind;

   union {
      bool           boolean;
      int            number;
      double         real;
      UChar          uchar;
      VirtualString* string;
      VirtualObject* object;
      VirtualArray*  array;
   } mValue;
};

#ifdef CRAFTER_INLINE
#  include "virtualvalue.h"
#endif

#endif // VIRTUAL_VALUE_H
