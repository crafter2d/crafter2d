
#include "type.h"

#include <map>

#include "core/defines.h"

namespace CIL
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
            String arraytypename = typestr.subStr(0, pos);
            auto arraytype = std::make_unique<Type>(fromString(arraytypename));

            result.mKind = Type::eArray;
            result.mInfo = ArrayInfo(std::move(arraytype));

            result.determineArrayDimension();
         }
         else
         {
            // todo, what with generics?
            if ( typestr[0] == '~' )
            {
               result.mKind = Type::eGeneric;
               result.mInfo = ObjectInfo(typestr.right(1));
            }
            else
            {
               result.mKind = Type::eObject;
               result.mInfo = ObjectInfo(typestr);
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

   Type::ArrayInfo::ArrayInfo(std::unique_ptr<Type> type):
      mType(std::move(type)),
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
      mKind(eNull),
      mInfo(ObjectInfo(String::empty()))
   {
   }
   
   Type::Type(Kind kind):
      mKind(kind),
      mInfo(ObjectInfo(String::empty()))
   {
      ASSERT(kind < eObject || kind == eNull);
   }

   Type::Type(Type&& that):
      mKind(that.mKind),
      mInfo(std::move(that.mInfo))
   {
   }

   Type::Type(const Type& that):
      mKind(that.mKind),
      mInfo(that.mInfo)
   {
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
      mKind = that.mKind;
      mInfo = that.mInfo;
      return *this;
   }

   Type& Type::operator=(Type&& that)
   {
      mKind = that.mKind;
      mInfo = std::move(that.mInfo);
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
      ASSERT(mKind == eObject || mKind == eGeneric);
      return std::get<ObjectInfo>(mInfo).mObjectName;
   }

   int Type::getArrayDimension() const
   {
      ASSERT(mKind == eArray);
      return std::get<ArrayInfo>(mInfo).mDimension;
   }

   const Type& Type::getArrayType() const
   {
      ASSERT(mKind == eArray);
      return *std::get<ArrayInfo>(mInfo).mType;
   }

   // - Operations

   void Type::determineArrayDimension()
   {
      ASSERT(isArray());

      auto& info = std::get<ArrayInfo>(mInfo);
      if ( info.mType->isArray() )
      {
         info.mType->determineArrayDimension();
         info.mDimension = info.mType->getArrayDimension();
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
            type += getObjectName();
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
            type = getArrayType().toString() + UTEXT("[]");
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
