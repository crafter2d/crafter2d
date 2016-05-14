
#ifndef TYPE_H
#define TYPE_H

#include <memory>

#include "core/string/string.h"

namespace yasc
{
   class Type
   {
   public:
      enum Kind { eNull, eInt, eReal, eBool, eChar, eString, eVoid, eObject, eArray, eGeneric };
      
      static const String sInt;
      static const String sReal;
      static const String sChar;
      static const String sString;
      static const String sBool;
      static const String sVoid;
      static const String sInvalid;

      static Type fromString(const String& typestr);

      Type();
      Type(Kind kind);
      Type(Type&& that);
      Type(const Type& that);
      ~Type();

      bool operator==(const Type& that) const;
      Type& operator=(const Type& that);
      Type& operator=(Type&& that);

    // query
      Kind getKind() const;

      bool isNull() const;
      bool isVoid() const;
      bool isBool() const;
      bool isInt() const;
      bool isReal() const;
      bool isChar() const;
      bool isString() const;
      bool isObject() const;
      bool isArray() const;

      bool equals(const Type& that) const;

    // object interface
      const String& getObjectName() const;

    // array interface
      int         getArrayDimension() const;
      const Type& getArrayType() const;

    // operations
      String toString() const;

   private:
      friend class TypeManager;

    // types
      struct ObjectInfo
      {
         String  mObjectName;
      };

      struct ArrayInfo
      {
         std::unique_ptr<Type> mType;
         int  mDimension;
      };

    // operations
      void determineArrayDimension();
      void move(Type&& that);

    // data      
      Kind mKind;
      union
      {
         ObjectInfo mObject;
         ArrayInfo  mArray;
      };
   };
}

#endif // TYPE_H
