
#ifndef VIRTUAL_VARIANT_H_
#define VIRTUAL_VARIANT_H_

#include <string>

#include "script/script_base.h"

class VirtualObject;
class VirtualArray;

class SCRIPT_API Variant
{
public:
            Variant();
            Variant(const Variant& that);
   explicit Variant(int value);
   explicit Variant(double value);
   explicit Variant(char value);
   explicit Variant(bool value);
   explicit Variant(const std::string& value);
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

   const std::string& asString() const;
   void setString(const std::string& value);

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
   std::string typeAsString() const;
   std::string toString() const;
   int     toInt() const;
   double  toReal() const;

 // conversion
   void int2real();
   void int2string();
   void real2int();
   void real2string();
   void char2string();
   void boolean2string();

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
   
   struct DataHolderBase
   {
      virtual ~DataHolderBase() {}

      virtual DataHolderBase* clone() const = 0;
   };

   template< typename T >
   struct DataHolder : DataHolderBase
   {
      DataHolder(const T& data): mData(data) {}
      DataHolder(const DataHolder& that): mData(that.mData) {}
      
      virtual DataHolder<T>* clone() const {
         return new DataHolder<T>(*this);
      }

      T mData;
   };

   typedef DataHolder<VirtualObject*> ObjectHolder;
   typedef DataHolder<VirtualArray*>  ArrayHolder;

   MetaType        mType;
   DataHolderBase* mpHolder;
};

#endif // VIRTUAL_VARIANT_H_
