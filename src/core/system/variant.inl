
#include "core/defines.h"

namespace c2d
{
   inline Variant::Variant():
      mKind(eEmpty),
      mData(),
      mStr()
   {
   }

   inline Variant::Variant(bool value) :
      mKind(eBool),
      mData(),
      mStr()
   {
      mData.mBool = value;
   }

   inline Variant::Variant(int value) :
      mKind(eInt),
      mData(),
      mStr()
   {
      mData.mInt = value;
   }

   inline Variant::Variant(float value) :
      mKind(eReal),
      mData(),
      mStr()
   {
      mData.mReal = value;
   }

   inline Variant::Variant(UChar value) :
      mKind(eChar),
      mData(),
      mStr()
   {
      mData.mChar = value;
   }

   inline Variant::Variant(const String& value) :
      mKind(eString),
      mData(),
      mStr(value)
   {
   }

   inline Variant::Variant(const Variant& that) :
      mKind(that.mKind),
      mData(that.mData),
      mStr(that.mStr)
   {
   }

   inline bool Variant::asBool() const
   {
      return mData.mBool;
   }

   inline int Variant::asInt() const
   {
      return mData.mInt;
   }

   inline float Variant::asReal() const
   {
      return mData.mReal;
   }

   inline UChar Variant::asChar() const
   {
      return mData.mChar;
   }
   
   inline String Variant::asString() const
   {
      return mStr;
   }

   inline bool Variant::isInt() const
   {
      return mKind == eInt;
   }

   inline bool Variant::isReal() const
   {
      return mKind == eReal;
   }

   inline bool Variant::isChar() const
   {
      return mKind == eChar;
   }

   inline bool Variant::isBool() const
   {
      return mKind == eBool;
   }

   inline bool Variant::isString() const
   {
      return mKind == eString;
   }

   inline void Variant::setBool(bool value)
   {
      mKind = eBool;
      mData.mBool = value;
   }

   inline void Variant::setInt(int value)
   {
      mKind = eInt;
      mData.mInt = value;
   }

   inline void Variant::setReal(float value)
   {
      mKind = eReal;
      mData.mReal = value;
   }

   inline void Variant::setChar(UChar value)
   {
      mKind = eChar;
      mData.mChar = value;
   }

   inline void Variant::setString(const String& value)
   {
      mKind = eString;
      mStr = value;
   }
}