
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
            auto* ptype = new Type(fromString(arraytype));

            result.mKind = Type::eArray;
            new (&result.mArray) ArrayInfo(ptype);
            result.determineArrayDimension();
         }
         else
         {
            // todo, what with generics?
            if ( typestr[0] == '~' )
            {
               result.mKind = Type::eGeneric;
               new (&result.mObject) ObjectInfo(typestr.right(1));
            }
            else
            {
               result.mKind = Type::eObject;
               new (&result.mObject) ObjectInfo(typestr);
            }
         }
      }

      ASSERT(result.mKind != eNull);
      return result;
   }

   // - ObjectInfo operators

   Type::ObjectInfo::ObjectInfo(String&& name) :
      mObjectName(std::move(name))
   {
   }

   Type::ObjectInfo::ObjectInfo(const String& name) :
      mObjectName(name)
   {
   }

   Type::ObjectInfo::ObjectInfo(ObjectInfo&& that):
      mObjectName(that.mObjectName)
   {
   }

   Type::ObjectInfo::ObjectInfo(const ObjectInfo& that):
      mObjectName(std::move(that.mObjectName))
   {
   }

   auto Type::ObjectInfo::operator=(const ObjectInfo& that) -> ObjectInfo&
   {
      mObjectName = that.mObjectName;
      return *this;
   }

   auto Type::ObjectInfo::operator=(ObjectInfo&& that) -> ObjectInfo&
   {
      mObjectName = std::move(that.mObjectName);
      return *this;
   }

   // - ArrayInfo operators

   Type::ArrayInfo::ArrayInfo(Type* ptype):
      mType(ptype),
      mDimension(1)
   {
   }

   Type::ArrayInfo::ArrayInfo(ArrayInfo&& that) :
      mType(std::move(that.mType)),
      mDimension(that.mDimension)
   {
   }

   Type::ArrayInfo::ArrayInfo(const ArrayInfo& that) :
      mType(new Type(*that.mType)),
      mDimension(that.mDimension)
   {
   }

   auto Type::ArrayInfo::operator=(ArrayInfo&& that) -> ArrayInfo&
   {
      mType = std::move(that.mType);
      mDimension = that.mDimension;
      return *this;
   }

   auto Type::ArrayInfo::operator=(const ArrayInfo& that) -> ArrayInfo&
   {
      mType.reset(that.mType ? new Type(*that.mType) : nullptr);
      mDimension = that.mDimension;
      return *this;
   }

   // - Implementation

   Type::Type():
      mKind(eNull)
   {
   }

   Type::Type(Kind kind):
      mKind(kind)
   {
      ASSERT(kind < eObject || kind == eNull);
   }

   Type::Type(Type&& that):
      mKind(that.mKind)
   {
      if ( mKind == eObject )
      {
         new (&mObject) ObjectInfo(std::move(that.mObject));
      }
      else if ( mKind == eArray )
      {
         new (&mArray) ArrayInfo(std::move(that.mArray));
      }
   }

   Type::Type(const Type& that):
      mKind(that.mKind)
   {
      if ( mKind == eObject )
      {
         new (&mObject) ObjectInfo(that.mObject);
      }
      else if ( mKind == eArray )
      {
         new (&mArray) ArrayInfo(that.mArray);
      }
   }

   Type::~Type()
   {
      if ( mKind == eObject )
      {
         mObject.~ObjectInfo();
      }
      else if ( mKind == eArray )
      {
         mArray.~ArrayInfo();
      }
   }

   bool Type::operator==(const Type& that) const
   {
      return equals(that);
   }

   Type& Type::operator=(const Type& that)
   {
      if ( that.mKind < eObject )
      {
         if ( mKind == eObject )
         {
            mObject.~ObjectInfo();
         }
         else if ( mKind == eArray )
         {
            mArray.~ArrayInfo();
         }
      }
      else if ( that.mKind == eObject )
      {
         if ( mKind != eObject )
         {
            if ( mKind == eArray )
            {
               mArray.~ArrayInfo();
            }
            new (&mObject) ObjectInfo(that.mObject);
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
            if ( mKind == eObject )
            {
               mObject.~ObjectInfo();
            }
            new (&mArray) ArrayInfo(that.mArray);
         }
         else
         {
            mArray = that.mArray;
         }
      }
      
      mKind = that.mKind;
      return *this;
   }

   Type& Type::operator=(Type&& that)
   {
      if ( that.mKind < eObject )
      {
         if ( mKind == eObject )
         {
            mObject.~ObjectInfo();
         }
         else if ( mKind == eArray )
         {
            mArray.~ArrayInfo();
         }
      }
      else if ( that.mKind == eObject )
      {
         if ( mKind != eObject )
         {
            if ( mKind == eArray )
            {
               mArray.~ArrayInfo();
            }
            new (&mObject) ObjectInfo(std::move(that.mObject));
         }
         else
         {
            mObject = std::move(that.mObject);
         }
      }
      else
      {
         ASSERT(that.mKind == eArray);
         if ( mKind != eArray )
         {
            if ( mKind == eObject )
            {
               mObject.~ObjectInfo();
            }
            new (&mArray) ArrayInfo(std::move(that.mArray));
         }
         else
         {
            mArray = std::move(that.mArray);
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
