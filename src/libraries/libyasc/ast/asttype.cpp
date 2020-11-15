
#include "asttype.h"

#include "core/defines.h"

#include "libyasc/compilecontext.h"

#include "astclass.h"
#include "astfunction.h"
#include "asttypevariable.h"
#include "asttypevariables.h"
#include "astvisitor.h"

static const String SInt = UTEXT("int");
static const String SReal = UTEXT("real");
static const String SChar = UTEXT("char");
static const String SString = UTEXT("string");
static const String SBool = UTEXT("boolean");
static const String SVoid = UTEXT("void");
static const String SInvalid = UTEXT("<unknown>");

ASTType ASTType::SVoidType(eVoid);

// static
ASTType ASTType::greaterType(const ASTType& left, const ASTType& right)
{
   if ( left.greater(right) )
      return right;
   else if ( right.greater(left) )
      return left;

   return ASTType();
}
   
// static 
ASTType* ASTType::fromString(const String& type)
{
   std::size_t pos = type.lastIndexOf('[');
   if ( pos != String::npos )
   {
      String arraytype = type.subStr(0, pos);
      ASTType* parray = new ASTType(ASTType::eArray);
      parray->setArrayType(fromString(arraytype));
      parray->determineArrayDimension();
      return parray;
   }

   if ( type == SBool )
   {
      return new ASTType(ASTType::eBoolean);
   }
   else if ( type == SInt  )
   {
      return new ASTType(ASTType::eInt);
   }
   else if ( type == SReal )
   {
      return new ASTType(ASTType::eReal);
   }
   else if ( type == SChar )
   {
      return new ASTType(ASTType::eChar);
   }
   else if ( type == SString )
   {
      return new ASTType(ASTType::eString);
   }
   else if ( type == SVoid )
   {
      return new ASTType(ASTType::eVoid);
   }
   else
   {
      String value;

      // todo, what with generics?
      ASTType::Kind kind = ASTType::eObject;
      if ( type[0] == '~' ) {
         kind = ASTType::eGeneric;
         value = type.subStr(1, type.length() - 1);
      }
      else
         value = type;

      ASTType* ptype = new ASTType(kind);
      ptype->setObjectName(value);
      return ptype;
   }
   return nullptr;
}

ASTType::ASTType():
   mKind(eInvalid),
   mPosition(),
   mObjectName(),
   mpObjectClass(nullptr),
   mTypeArguments(),
   mpTypeVariable(nullptr),
   mArrayDimension(0),
   mpArrayType(nullptr)
{
}

ASTType::ASTType(Kind kind):
   mKind(kind),
   mPosition(),
   mObjectName(),
   mpObjectClass(nullptr),
   mTypeArguments(),
   mpTypeVariable(nullptr),
   mArrayDimension(0),
   mpArrayType(nullptr)
{
}

ASTType::ASTType(const ASTType& that):
   mKind(that.mKind),
   mPosition(that.mPosition),
   mObjectName(that.mObjectName),
   mpObjectClass(that.mpObjectClass),
   mTypeArguments(that.mTypeArguments),
   mpTypeVariable(that.mpTypeVariable),
   mArrayDimension(that.mArrayDimension),
   mpArrayType(that.mpArrayType != nullptr ? that.mpArrayType->clone() : nullptr)
{
}

ASTType::~ASTType()
{
   clear();
}

const ASTType& ASTType::operator=(const ASTType& that)
{
   mKind = that.mKind;
   mObjectName = that.mObjectName;
   mpObjectClass = that.mpObjectClass;
   mArrayDimension = that.mArrayDimension;
   mpArrayType = that.mpArrayType != nullptr ? that.mpArrayType->clone() : nullptr;
   mTypeArguments = that.mTypeArguments;
   mpTypeVariable = that.mpTypeVariable;
   mPosition = that.mPosition;

   return *this;
}

// - Get/set

ASTType::Kind ASTType::getKind() const
{
   return mKind;
}

void ASTType::setKind(Kind kind)
{
   mKind = kind;
}

const String& ASTType::getObjectName() const
{
   return mObjectName;
}

void ASTType::setObjectName(const String& name)
{
   mObjectName = name;
}

bool ASTType::hasObjectClass() const
{
   return mpObjectClass != nullptr;
}

const ASTClass& ASTType::getObjectClass() const
{
   ASSERT_PTR(mpObjectClass);
   return *mpObjectClass;
}

ASTClass& ASTType::getObjectClass()
{
   ASSERT_PTR(mpObjectClass);
   return *mpObjectClass;
}

void ASTType::setObjectClass(ASTClass& aclass)
{
   mpObjectClass = &aclass;
}

int ASTType::getArrayDimension() const
{
   return mArrayDimension;
}

void ASTType::setArrayDimension(int dimension)
{
   mArrayDimension = dimension;
}

const ASTType& ASTType::getArrayType() const
{
   ASSERT(isArray());
   ASSERT_PTR(mpArrayType);
   return *mpArrayType;
}

ASTType& ASTType::getArrayType()
{
   ASSERT(isArray());
   ASSERT_PTR(mpArrayType);
   return *mpArrayType;
}

void ASTType::setArrayType(ASTType* ptype)
{
   delete mpArrayType;
   mpArrayType = ptype;
}

const ASTTypeList& ASTType::getTypeArguments() const
{
   return mTypeArguments;
}

ASTTypeList& ASTType::getTypeArguments()
{
   return mTypeArguments;
}

const ASTTypeVariable& ASTType::getTypeVariable() const
{
   ASSERT(isGeneric());
   if ( mKind == eArray )
   {
      return mpArrayType->getTypeVariable();
   }
   return *mpTypeVariable;
}

void ASTType::setTypeVariable(const ASTTypeVariable& typevariable)
{
   mpTypeVariable = &typevariable;
}

const AntlrTextPosition& ASTType::getPosition() const
{
   return mPosition;
}

void ASTType::setPosition(const AntlrTextPosition& pos)
{
   mPosition = pos;
}

// - Query

bool ASTType::isValid() const
{
   return mKind != eInvalid;
}

bool ASTType::isGeneric() const
{
   if ( mKind == eArray )
   {
      return mpArrayType->isGeneric();
   }
   return mKind == eGeneric;
}

bool ASTType::isVoid() const
{
   return mKind == eVoid;
}

bool ASTType::isNumeric() const
{
   return isInt() || isReal();
}

bool ASTType::isUnknown() const
{
   return mKind == eUnknown;
}

bool ASTType::isValueType() const
{
   return isNumeric() || isChar() || isString();
}

bool ASTType::isNull() const
{
   return mKind == eNull;
}

bool ASTType::isArray() const
{
   return mKind == eArray;
}

bool ASTType::isObject() const
{
   return mKind == eObject;
}

bool ASTType::isBoolean() const
{
   return mKind == eBoolean;
}

bool ASTType::isInt() const
{
   return mKind == eInt;
}

bool ASTType::isReal() const
{
   return mKind == eReal;
}

bool ASTType::isChar() const
{
   return mKind == eChar;
}

bool ASTType::isString() const
{
   return mKind == eString;
}

bool ASTType::equals(const ASTType& that) const
{
   if ( (isObject() || isArray()) && that.isNull() )
   {
      return true;
   }

   return mKind == that.mKind
       && (isObject() ? mpObjectClass == that.mpObjectClass : true);
}

/// \brief Test whether that is greater than this type
bool ASTType::greater(const ASTType& that) const
{
   if ( isNull() && (that.isObject() || that.isArray() || that.isString() || that.isGeneric()) )
   {
      return true;
   }
   else if ( isObject() && that.isObject() )
   {
      // check if 'that' is a extending or implemented this

      if ( !(mTypeArguments == that.mTypeArguments) )
      {
         return false;
      }

      return getObjectClass().isBase(that.getObjectClass()) 
          || getObjectClass().isImplementing(that.getObjectClass());
   }
   else if ( isArray() && that.isArray() )
   {
      return mpArrayType->equals(*that.mpArrayType) && mArrayDimension == that.mArrayDimension;
   }
   else if ( isGeneric() )
   {
      if ( that.isObject() )
      {
          return that.getObjectName() == UTEXT("system.Object"); // object is greater than a generic (its da uber type)
      }
      else if ( that.isGeneric() )
      {
         return mObjectName == that.mObjectName;
      }
   }
   else if ( !isObject() && !that.isObject() )
   {
      switch ( that.mKind )
      {
         case eBoolean:
            return mKind == eBoolean;

         case eInt:
            return mKind == eInt;

         case eReal:
            return mKind == eInt || mKind == eReal;

         case eChar:
            return mKind == eChar;

         case eString:
            return mKind == eString || mKind == eInt || mKind == eReal || mKind == eBoolean || mKind == eChar;
            
         default:
            break;
      }
   }

   // no implicit primitive to basic or vs yet

   return false;
}

bool ASTType::isDerivedFrom(const ASTType& that) const
{
   if ( !isUnknown() && !that.isUnknown() )
   {
      return that.getObjectClass().isBase(getObjectClass())
          || that.getObjectClass().isImplementing(getObjectClass());
   }

   return false;
}

// - Operations

void ASTType::addTypeArgument(ASTType* ptype)
{
   mTypeArguments.append(ptype);
}

void ASTType::replaceArgument(const ASTType& type)
{
   mTypeArguments[0] = type;
}

bool ASTType::resolveType(CompileContext& context, const ASTTypeVariables* ptypevariables)
{
   if ( ptypevariables != nullptr )
   {
      const ASTTypeVariable* ptypevariable = ptypevariables->find(mObjectName);
      if ( ptypevariable != nullptr )
      {
         mKind = ASTType::eGeneric;
         mpTypeVariable = ptypevariable;

         return true;
      }
   }
      
   if ( mKind == eObject )
   {
      for ( int index = 0; index < mTypeArguments.size(); index++ )
      {
         if ( !mTypeArguments[index].resolveType(context, ptypevariables) )
         {
            return false;
         }
      }

      mpObjectClass = &context.resolveClass(mObjectName);
   }
   else if ( mKind == eArray )
   {
      mpObjectClass = &context.resolveClass(UTEXT("system.InternalArray"));
      return mpArrayType->resolveType(context, ptypevariables);
   }
   else if ( mKind == eString )
   {
      mpObjectClass = &context.resolveClass(UTEXT("system.InternalString"));
   }

   return true;
}

void ASTType::clear()
{
   mKind = eInvalid;

   mpObjectClass = nullptr;
   setArrayType(nullptr);

   mTypeArguments.clear();
}

ASTType* ASTType::clone() const
{
   return new ASTType(*this);
}

void ASTType::determineArrayDimension()
{
   mArrayDimension = 1;
   ASTType* ptype = mpArrayType;
   while ( ptype->isArray() )
   {
      ptype = ptype->mpArrayType;
      mArrayDimension++;
   }
}

// - Conversion

String ASTType::toString() const
{
   String type;

   switch ( mKind )
   {
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
      case eBoolean:
         type = SBool;
         break;
      case eGeneric:
         type = '~';
         // fall through
      case eObject:
         type += mpObjectClass != nullptr ? mpObjectClass->getFullName() : mObjectName;
         if ( !mTypeArguments.empty() )
         {
            type += '<';
            for ( int index = 0; index < mTypeArguments.size(); index++ )
            {
               const ASTType& arg = mTypeArguments[index];
               type += arg.toString();
            }
            type += '>';
         }
         break;
      case eArray:
         type = mpArrayType->toString();
         break;
      case eVoid:
         type = SVoid;
         break;

      case eInvalid:
      default:
         type = SInvalid;
   }

   if ( isArray() )
   {
      type += UTEXT("[]");
   }

   return type;
}
