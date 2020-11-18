
#ifndef TYPE_H
#define TYPE_H

#include <memory>
#include <variant>

#include "core/string/string.h"

namespace yasc
{
   class Type
   {
   public:
      enum Kind { eNull, eInt, eReal, eBool, eChar, eString, eVoid, eGeneric, eObject, eArray };
      
      static const String sInt;
      static const String sReal;
      static const String sChar;
      static const String sString;
      static const String sBool;
      static const String sVoid;
      static const String sInvalid;

      static Type fromString(const String& typestr);

      Type();
      Type(Type&& that);
      Type(Kind kind);
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
         ObjectInfo(String&& name);
         ObjectInfo(const String& name);
         ObjectInfo(ObjectInfo&& that);
         ObjectInfo(const ObjectInfo& that);
         ObjectInfo& operator=(ObjectInfo&& that);
         ObjectInfo& operator=(const ObjectInfo& that);
         ~ObjectInfo() {}

         String  mObjectName;
      };

      struct ArrayInfo
      {
         ArrayInfo(std::unique_ptr<Type> type);
         ArrayInfo(ArrayInfo&& that);
         ArrayInfo(const ArrayInfo& that);
         ArrayInfo& operator=(ArrayInfo&& that);
         ArrayInfo& operator=(const ArrayInfo& that);
         ~ArrayInfo() {}

         std::unique_ptr<Type> mType;
         int  mDimension;
      };

    // operations
      void determineArrayDimension();
      void move(Type&& that);

    // data      
      Kind mKind;
      std::variant<ArrayInfo, ObjectInfo> mInfo;
   };
}

#endif // TYPE_H
