
#ifndef VIRTUAL_VARIANT_H_
#define VIRTUAL_VARIANT_H_

#include <string>

class VirtualObjectReference;
class VirtualArrayReference;

class Variant
{
public:
            Variant();
            Variant(const Variant& that);
   explicit Variant(int value);
   explicit Variant(double value);
   explicit Variant(bool value);
   explicit Variant(const std::string& value);
   explicit Variant(const VirtualObjectReference& object);
   explicit Variant(const VirtualArrayReference& array);
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

   const std::string& asString() const;
   void setString(const std::string& value);

   VirtualObjectReference& asObject() const;
   void setObject(const VirtualObjectReference& object);

   VirtualArrayReference& asArray() const;
   void setArray(const VirtualArrayReference& array);

 // query
   bool isEmpty() const;
   bool isInt() const;
   bool isReal() const;
   bool isBool() const;
   bool isString() const;
   bool isObject() const;
   bool isArray() const;

 // display
   std::string toString() const;
   int     toInt() const;
   double  toReal() const;

 // conversion
   void int2real();
   void int2string();
   void real2string();

private:
   enum MetaType
   {
      eEmpty,
      eInt,
      eReal,
      eString,
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

   MetaType        mType;
   DataHolderBase* mpHolder;
};

#endif // VIRTUAL_VARIANT_H_
