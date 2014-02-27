
#ifndef TYPE_H
#define TYPE_H

#include "core/string/string.h"

namespace yasc
{
   class Type
   {
   public:
      enum Kind { eInt, eReal, eBool, eChar, eString, eObject, eArray, eGeneric, eVoid, eNull };

      static Type* fromString(const String& typestr);
      static Type* newObjectType(const String& classname);

      Type();
      Type(Kind kind);
      Type(const Type& that);
      ~Type();

      const Type& operator=(const Type& that);

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
      void          setObjectName(const String& name);

      // array interface
      int         getArrayDimension() const;
      const Type& getArrayType() const;

      // operations
      void assign(const Type& that);
      Type* clone() const;
      String toString() const;

   private:

      // types
      struct ObjectInfo
      {
         String  mObjectName;
      };

      struct ArrayInfo
      {
         Type*   mpType;
         int     mDimension;
      };

      union Info
      {
         ObjectInfo*    mpObject;
         ArrayInfo*     mpArray;
      };

      // operations
      void determineArrayDimension();

      // data
      Kind  mKind;
      Info  mInfo;
   };
}

#endif // TYPE_H
