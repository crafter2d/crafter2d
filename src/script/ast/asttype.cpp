
#include "asttype.h"

#include "core/defines.h"

#include "script/compiler/compilecontext.h"
#include "script/compiler/classresolver.h"

#include "astclass.h"
#include "asttypevariable.h"
#include "asttypevariables.h"
#include "astvisitor.h"

static const std::string SInt = "int";
static const std::string SReal = "real";
static const std::string SString = "string";
static const std::string SBool = "boolean";
static const std::string SInvalid = "<unknown>";

ASTType ASTType::SVoidType(eVoid);

// static
ASTType ASTType::greaterType(const ASTType& left, const ASTType& right)
{
   if ( left.greater(right) )
      return left;
   else if ( right.greater(left) )
      return right;

   return ASTType();
}

ASTType::ASTType():
   mKind(eInvalid),
   mObjectName(),
   mpObjectClass(NULL),
   mTypeArguments(),
   mpTypeVariable(NULL),
   mArrayDimension(0),
   mpArrayType(NULL),
   mLine(-1),
   mPos(-1)
{
}

ASTType::ASTType(Kind kind):
   mKind(kind),
   mObjectName(),
   mpObjectClass(NULL),
   mTypeArguments(),
   mpTypeVariable(NULL),
   mArrayDimension(0),
   mpArrayType(NULL),
   mLine(-1),
   mPos(-1)
{
}

ASTType::ASTType(const ASTType& that):
   mKind(that.mKind),
   mObjectName(that.mObjectName),
   mpObjectClass(that.mpObjectClass),
   mTypeArguments(that.mTypeArguments),
   mpTypeVariable(that.mpTypeVariable),
   mArrayDimension(that.mArrayDimension),
   mpArrayType(that.mpArrayType != NULL ? that.mpArrayType->clone() : NULL),
   mLine(that.mLine),
   mPos(that.mPos)
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
   mpArrayType = that.mpArrayType != NULL ? that.mpArrayType->clone() : NULL;
   mTypeArguments = that.mTypeArguments;
   mpTypeVariable = that.mpTypeVariable;
   mLine = that.mLine;
   mPos = that.mPos;

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

const std::string& ASTType::getObjectName() const
{
   return mObjectName;
}

void ASTType::setObjectName(const std::string& name)
{
   mObjectName = name;
}

bool ASTType::hasObjectClass() const
{
   return mpObjectClass != NULL;
}

const ASTClass& ASTType::getObjectClass() const
{
   ASSERT(mpObjectClass != NULL);
   return *mpObjectClass;
}

ASTClass& ASTType::getObjectClass()
{
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

void ASTType::setArrayType(ASTType* ptype)
{
   delete mpArrayType;
   mpArrayType = ptype;
}

const ASTTypeList& ASTType::getTypeArguments() const
{
   return mTypeArguments;
}

const ASTTypeVariable& ASTType::getTypeVariable() const
{
   ASSERT(isGeneric());
   return *mpTypeVariable;
}

void ASTType::setTypeVariable(const ASTTypeVariable& typevariable)
{
   mpTypeVariable = &typevariable;
}

int ASTType::getLine() const
{
   return mLine;
}

void ASTType::setLine(int line)
{
   mLine = line;
}

int ASTType::getPos() const
{
   return mPos;
}

void ASTType::setPos(int pos)
{
   mPos = pos;
}

// - Query

bool ASTType::isValid() const
{
   return mKind != eInvalid;
}

bool ASTType::isGeneric() const
{
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

bool ASTType::isString() const
{
   return mKind == eString;
}

bool ASTType::equals(const ASTType& that) const
{
   return mKind == that.mKind
       && (isObject() ? mpObjectClass == that.mpObjectClass : true);
}

bool ASTType::greater(const ASTType& that) const
{
   if ( isObject() && that.isObject() )
   {
      // check if 'that' is a base class or implemented class
      const ASTClass& thatclass = that.getObjectClass();

      if ( !(mTypeArguments == that.mTypeArguments) )
      {
         return false;
      }
      
      return getObjectClass().isBase(thatclass) || getObjectClass().isImplementing(thatclass);
   }
   else if ( isArray() && that.isArray() )
   {
      return mpArrayType->equals(*that.mpArrayType) && mArrayDimension == that.mArrayDimension;
   }
   else if ( isGeneric() )
   {
      if ( that.isObject() )
      {
          return that.getObjectName() == "Object"; // object is greater than a generic (its da uber type)
      }
      else if ( that.isGeneric() )
      {
         return mObjectName == that.mObjectName;
      }
   }
   else if ( !isObject() && !that.isObject() )
   {
      Kind kind = that.mKind;

      switch ( mKind )
      {
         case eBoolean:
            return kind == eBoolean;
            
         case eInt:
            return kind == eInt;

         case eReal:
            return kind == eInt || kind == eReal;

         case eString:
            return kind == eString || kind == eInt || kind == eReal;
      }
   }

   // no implicit primitive to basic or vs yet

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

bool ASTType::resolveType(CompileContext& context, const ClassResolver& resolver, const ASTClass& aclass)
{
   const ASTTypeVariable* ptypevariable = aclass.isGeneric() ? aclass.getTypeVariables().find(mObjectName) : NULL;
   if ( ptypevariable != NULL )
   {
      mKind = ASTType::eGeneric;
      mpTypeVariable = ptypevariable;
   }
   else if ( mKind == eObject )
   {
      for ( int index = 0; index < mTypeArguments.size(); index++ )
      {
         if ( !mTypeArguments[index].resolveType(context, resolver, aclass) )
         {
            return false;
         }
      }

      if ( mObjectName == aclass.getName() )
      {
         mpObjectClass = const_cast<ASTClass*>(&aclass);
         return true;
      }
      else
      {
         std::string fullname = resolver.resolve(mObjectName);
         if ( fullname.empty() )
         {
            return false;
         }

         if ( mpObjectClass == NULL )
         {
            mpObjectClass = context.findClass(fullname);
            return mpObjectClass != NULL;
         }
      }
   }
   else if ( mKind == eArray )
   {
      mpObjectClass = context.findClass("InternalArray");
      return mpArrayType->resolveType(context, resolver, aclass);
   }

   return true;
}

void ASTType::clear()
{
   mKind = eInvalid;

   mpObjectClass = NULL;
   setArrayType(NULL);

   mTypeArguments.clear();
}

ASTType* ASTType::clone() const
{
   return new ASTType(*this);
}

// - Conversion
   
std::string ASTType::toString() const
{
   std::string type;

   switch ( mKind )
   {
      case eInt:
         type = SInt;
         break;
      case eReal:
         type = SReal;
         break;
      case eString:
         type = SString;
         break;
      case eBoolean:
         type = SBool;
         break;
      case eObject:
         type = mObjectName;
         if ( !mTypeArguments.empty() )
         {
            type += "<";
            for ( int index = 0; index < mTypeArguments.size(); index++ )
            {
               const ASTType& arg = mTypeArguments[index];
               type += arg.toString();
            }
            type += ">";
         }
         break;
      case eArray:
         type = "array";
         break;

      case eInvalid:
      default:
         type = SInvalid;
   }

   if ( isArray() )
   {
      type += "[]";
   }

   return type;
}
