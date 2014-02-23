
#include "type.h"

#include "core/defines.h"

namespace yasc
{

   static const String SInt = UTEXT("int");
   static const String SReal = UTEXT("real");
   static const String SChar = UTEXT("char");
   static const String SString = UTEXT("string");
   static const String SBool = UTEXT("boolean");
   static const String SVoid = UTEXT("void");
   static const String SInvalid = UTEXT("<unknown>");

   // - Statics

   Type* Type::fromString(const String& typestr)
   {
      int pos = typestr.lastIndexOf('[');
      if ( pos != -1 )
      {
         String arraytype = typestr.subStr(0, pos);

         ArrayInfo* pinfo = new ArrayInfo;
         pinfo->mDimension = 1;
         pinfo->mpType = fromString(arraytype);

         Type* parray = new Type(Type::eArray);
         parray->mInfo.mpArray = pinfo;
         parray->determineArrayDimension();
         return parray;
      }

      if ( typestr == SBool )
      {
         return new Type(Type::eBool);
      }
      else if ( typestr == SInt  )
      {
         return new Type(Type::eInt);
      }
      else if ( typestr == SReal )
      {
         return new Type(Type::eReal);
      }
      else if ( typestr == SChar )
      {
         return new Type(Type::eChar);
      }
      else if ( typestr == SString )
      {
         return new Type(Type::eString);
      }
      else if ( typestr == SVoid )
      {
         return new Type(Type::eVoid);
      }
      else
      {
         ObjectInfo* pinfo = new ObjectInfo;

         // todo, what with generics?
         Type::Kind kind = eObject;
         if ( typestr[0] == '~' )
         {
            kind = Type::eGeneric;
            pinfo->mObjectName = typestr.subStr(1, typestr.length() - 1);
         }
         else
         {
            pinfo->mObjectName = typestr;
         }
      
         Type* ptype = new Type(kind);
         ptype->mInfo.mpObject = pinfo;
         return ptype;
      }
      return NULL;
   }

   // static 
   Type* Type::newObjectType(const String& classname)
   {
      Type* presult = new Type(eObject);
      presult->setObjectName(classname);
      return presult;
   }

   // - Implementation

   Type::Type():
      mKind(eNull),
      mInfo()
   {
   }

   Type::Type(Kind kind):
      mKind(kind),
      mInfo()
   {
   }

   Type::Type(const Type& that):
      mKind(eNull),
      mInfo()
   {
      assign(that);
   }

   Type::~Type()
   {
      switch ( mKind )
      {
      case eObject:
         delete mInfo.mpObject;
         break;
      case eArray:
         delete mInfo.mpArray;
         break;
      }
   }

   const Type& Type::operator=(const Type& that)
   {
      assign(that);
      return *this;
   }

   void Type::assign(const Type& that)
   {
      mKind = that.mKind;
      if ( mKind == eObject )
      {
         mInfo.mpObject = new ObjectInfo();
         mInfo.mpObject->mObjectName = that.mInfo.mpObject->mObjectName;
      }
      else if ( mKind == eArray )
      {
         ArrayInfo& thatarray = *that.mInfo.mpArray;

         mInfo.mpArray = new ArrayInfo();
         mInfo.mpArray->mDimension = thatarray.mDimension;
         mInfo.mpArray->mpType = thatarray.mpType->clone();
      }
   }

   Type* Type::clone() const
   {
      Type* presult = new Type();
      presult->assign(*this);
      return presult;
   }

   // - Query

   bool Type::equals(const Type& that) const
   {
      if ( mKind != that.mKind )
         return false;

      if ( mKind == eObject )
      {
         return getObjectName() != that.getObjectName();
      }
      else if ( mKind == eArray )
      {
         return getArrayDimension() == that.getArrayDimension()
               && getArrayType().equals(that.getArrayType());
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
      return mInfo.mpObject->mObjectName;
   }

   void Type::setObjectName(const String& name)
   {
      ASSERT(mKind == eObject);
      mInfo.mpObject->mObjectName = name;
   }

   int Type::getArrayDimension() const
   {
      ASSERT(mKind == eArray);
      return mInfo.mpArray->mDimension;
   }

   const Type& Type::getArrayType() const
   {
      ASSERT(mKind == eArray);
      return *mInfo.mpArray->mpType;
   }

   // - Operations

   void Type::determineArrayDimension()
   {
      ASSERT(isArray());

      ArrayInfo& info = *mInfo.mpArray;
      if ( info.mpType->isArray() )
      {
         info.mpType->determineArrayDimension();
         info.mDimension = info.mpType->getArrayDimension();
      }
   }

   String Type::toString() const
   {
      String type;

      switch ( mKind )
      {
         case eBool:
            type = SBool;
            break;
         case eInt:
            type = SInt;
            break;
         case eReal:
            type = SReal;
            break;
         case eChar:
            type = SChar;
            break;
         case eString:
            type = SString;
            break;
         case eGeneric:
            type = '~';
            // fall through
         case eObject:
            type += mInfo.mpObject->mObjectName;
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
            type = mInfo.mpArray->mpType->toString() + UTEXT("[]");
            break;
         case eVoid:
            type = SVoid;
            break;
         default:
            type = SInvalid;
      }

      return type;
   }

}
