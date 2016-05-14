
#include "type.h"

#include <map>

#include "core/defines.h"

namespace yasc
{
   // - Statics

   const String Type::sInt(L"int");
   const String Type::sReal(L"real");
   const String Type::sChar(L"char");
   const String Type::sString(L"string");
   const String Type::sBool(L"boolean");
   const String Type::sVoid(L"void");
   const String Type::sInvalid(L"<unknown>");

   static std::map<String, Type::Kind> sTypeMap = {
      { Type::sInt, Type::eInt },
      { Type::sReal, Type::eReal },
      { Type::sChar, Type::eChar },
      { Type::sString, Type::eString },
      { Type::sBool, Type::eBool },
      { Type::sVoid, Type::eVoid },
   };

   Type Type::fromString(const String& typestr)
   {
      Type result;

      auto it = sTypeMap.find(typestr);
      if ( it != sTypeMap.end() )
      {
         result.mKind = it->second;
      }
      else
      {
         std::size_t pos = typestr.lastIndexOf('[');
         if ( pos != String::npos )
         {
            String arraytype = typestr.subStr(0, pos);

            result.mKind = Type::eArray;
            result.mObject.mObjectName.~String();
            new (&result.mArray.mType) std::unique_ptr<Type>(new Type(fromString(arraytype)));
            result.mArray.mDimension = 1;
            result.determineArrayDimension();
         }
         else
         {
            // todo, what with generics?
            result.mKind = Type::eObject;
            if ( typestr[0] == '~' )
            {
               result.mKind = Type::eGeneric;
               result.mObject.mObjectName = typestr.right(1);
            }
            else
            {
               result.mObject.mObjectName = typestr;
            }
         }
      }

      ASSERT(result.mKind != eNull);
      return result;
   }

   // - Implementation

   Type::Type():
      mKind(eNull),
      mObject()
   {
   }

   Type::Type(Kind kind):
      mKind(kind),
      mObject()
   {
      ASSERT(kind < eObject || kind == eNull);
   }

   Type::Type(Type&& that):
      mKind(eNull),
      mObject()
   {
      operator=(std::move(that));
   }

   Type::Type(const Type& that):
      mKind(eNull),
      mObject()
   {
      operator=(that);
   }

   Type::~Type()
   {
   }

   bool Type::operator==(const Type& that) const
   {
      return equals(that);
   }

   Type& Type::operator=(const Type& that)
   {
      if ( that.mKind == eObject )
      {
         if ( mKind == eArray )
         {
            mArray.mType.std::unique_ptr<Type>::~unique_ptr();
            new (&mObject.mObjectName) String(that.mObject.mObjectName);
         }
         else
         {
            mObject.mObjectName = that.mObject.mObjectName;
         }
      }
      else if ( that.mKind == eArray )
      {
         if ( mKind != eArray )
         {
            mObject.mObjectName.~String();
            new (&mArray.mType) std::unique_ptr<Type>(new Type(*that.mArray.mType));
         }
         else
         {
            mArray.mType.reset(new Type(*that.mArray.mType));
         }
         mArray.mDimension = that.mArray.mDimension;
      }
      else
      {
         if ( mKind == eArray )
         {
            mArray.mType.std::unique_ptr<Type>::~unique_ptr();
            new (&mObject.mObjectName) String();
         }
      }
      mKind = that.mKind;
      return *this;
   }

   Type& Type::operator=(Type&& that)
   {
      if ( that.mKind == eObject )
      {
         if ( mKind == eArray )
         {
            mArray.mType.std::unique_ptr<Type>::~unique_ptr();
            new (&mObject.mObjectName) String(std::move(that.mObject.mObjectName));
         }
         else
         {
            mObject.mObjectName = std::move(that.mObject.mObjectName);
         }
      }
      else if ( that.mKind == eArray )
      {
         if ( mKind != eArray )
         {
            mObject.mObjectName.~String();
            new (&mArray.mType) std::unique_ptr<Type>(std::move(that.mArray.mType));
         }
         else
         {
            mArray.mType.swap(that.mArray.mType);
         }
         mArray.mDimension = that.mArray.mDimension;
      }
      else
      {
         if ( mKind == eArray )
         {
            mArray.mType.std::unique_ptr<Type>::~unique_ptr();
            new (&mObject.mObjectName) String();
         }
      }
      mKind = that.mKind;
      return *this;
   }

   // - Query

   bool Type::equals(const Type& that) const
   {
      if ( mKind != that.mKind )
         return false;

      if ( mKind == eObject )
      {
         return getObjectName() == that.getObjectName();
      }
      else if ( mKind == eArray )
      {
         return getArrayDimension() == that.getArrayDimension()
             && getArrayType() == that.getArrayType();
      }

      return true;
   }

   Type::Kind Type::getKind() const
   {
      return mKind;
   }

   bool Type::isNull() const
   {
      return mKind == eNull;
   }

   bool Type::isVoid() const
   {
      return mKind == eVoid;
   }

   bool Type::isBool() const
   {
      return mKind == eBool;
   }

   bool Type::isInt() const
   {
      return mKind == eInt;
   }

   bool Type::isReal() const
   {
      return mKind == eReal;
   }

   bool Type::isChar() const
   {
      return mKind == eChar;
   }

   bool Type::isString() const
   {
      return mKind == eString;
   }

   bool Type::isObject() const
   {
      return mKind == eObject;
   }

   bool Type::isArray() const
   {
      return mKind == eArray;
   }

   const String& Type::getObjectName() const
   {
      ASSERT(mKind == eObject);
      return mObject.mObjectName;
   }

   int Type::getArrayDimension() const
   {
      ASSERT(mKind == eArray);
      return mArray.mDimension;
   }

   const Type& Type::getArrayType() const
   {
      ASSERT(mKind == eArray);
      return *mArray.mType;
   }

   // - Operations

   void Type::determineArrayDimension()
   {
      ASSERT(isArray());

      if ( mArray.mType->isArray() )
      {
         mArray.mType->determineArrayDimension();
         mArray.mDimension = mArray.mType->getArrayDimension();
      }
   }

   String Type::toString() const
   {
      String type;

      switch ( mKind )
      {
         case eBool:
            type = sBool;
            break;
         case eInt:
            type = sInt;
            break;
         case eReal:
            type = sReal;
            break;
         case eChar:
            type = sChar;
            break;
         case eString:
            type = sString;
            break;
         case eGeneric:
            type = '~';
            // fall through
         case eObject:
            type += mObject.mObjectName;
            /*
            if ( !mTypeArguments.empty() )
            {
               type += '<';
               for ( int index = 0; index < mTypeArguments.size(); index++ )
               {
                  const ASTType& arg = mTypeArguments[index];
                  type += arg.toString();
               }
               type += '>';
            }*/
            break;
         case eArray:
            type = mArray.mType->toString() + UTEXT("[]");
            break;
         case eVoid:
            type = sVoid;
            break;
         default:
            type = sInvalid;
      }

      return type;
   }
}
