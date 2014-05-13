
#include "asttype.h"

#include "core/string/string.h"
#include "core/defines.h"

#include "asteffect.h"

static const String sVoid(UTEXT("void"));

// DirectX HSLS
static const String sFloat(UTEXT("float"));
static const String sFloat2(UTEXT("float2"));
static const String sFloat3(UTEXT("float3"));
static const String sFloat4(UTEXT("float4"));
static const String sMat2x2(UTEXT("float2x2"));
static const String sMat3x3(UTEXT("float3x3"));
static const String sMat4x4(UTEXT("float4x4"));

// OpenGL GLSL
static const String sVec2(UTEXT("vec2"));
static const String sVec3(UTEXT("vec3"));
static const String sVec4(UTEXT("vec4"));
static const String sMat2(UTEXT("mat2"));
static const String sMat3(UTEXT("mat3"));
static const String sMat4(UTEXT("mat4"));

// static 
ASTType* ASTType::fromString(const ASTEffect& effect, const String& typestr)
{
   ASTType* presult = new ASTType();

   if ( typestr == sFloat )
   {
      presult->mType = ASTType::eFloat;
   }
   else if ( typestr == sFloat2 || typestr == sVec2 )
   {
      presult->mType = ASTType::eFloat2;
   }
   else if ( typestr == sFloat3 || typestr == sVec3 )
   {
      presult->mType = ASTType::eFloat3;
   }
   else if ( typestr == sFloat4 || typestr == sVec4 )
   {
      presult->mType = ASTType::eFloat4;
   }
   else if ( typestr == sMat2x2 || typestr == sMat2 )
   {
      presult->mType = ASTType::eMat2;
   }
   else if ( typestr == sMat3x3 || typestr == sMat3 )
   {
      presult->mType = ASTType::eMat3;
   }
   else if ( typestr == sMat4x4 || typestr == sMat4 )
   {
      presult->mType = ASTType::eMat4;
   }
   else if ( typestr == sVoid )
   {
      presult->mType = ASTType::eVoid;
   }
   else
   {
      const ASTStruct* pstruct = effect.findStruct(typestr);
      if ( pstruct != NULL )
      {
         presult->mType = ASTType::eStruct;
         presult->mpStruct = pstruct;
      }
   }

   return presult;
}

ASTType::ASTType():
   mType(eUnknown),
   mTemplateClass(),
   mpStruct(NULL)
{
}

ASTType::ASTType(Type type):
   mType(type),
   mTemplateClass(),
   mpStruct(NULL)
{
}

// - Get/set

ASTType::Type ASTType::getType() const
{
   return mType;
}

const String& ASTType::getTemplateClass() const
{
   return mTemplateClass;
}

void ASTType::setTemplateClass(const String tmplclass)
{
   mTemplateClass = tmplclass;
}

const ASTStruct& ASTType::getStruct() const
{
   ASSERT(mType == eStruct);
   ASSERT_PTR(mpStruct);
   return *mpStruct;
}

// - Query

bool ASTType::isStruct() const
{
   return mType == eStruct;
}

bool ASTType::isUnknown() const
{
   return mType == eUnknown;
}

// - Conversion

String ASTType::toDirectX() const
{
   String result;
   
   switch ( mType )
   {
   case ASTType::eFloat:
      result = sFloat;
      break;
   case ASTType::eFloat2:
      result = sFloat2;
      break;
   case ASTType::eFloat3:
      result = sFloat3;
      break;
   case ASTType::eFloat4:
      result = sFloat4;
      break;
   case ASTType::eMat2:
      result = sMat2x2;
      break;
   case ASTType::eMat3:
      result = sMat3x3;
      break;
   case ASTType::eMat4:
      result = sMat4x4;
      break;
   case ASTType::eStruct:
      result = mpStruct->mName;
      break;

   case ASTType::eVoid:
      return sVoid;
   
   default:
      throw new std::exception("Invalid type!");
   }

   if ( !mTemplateClass.isEmpty() )
   {
      result = mTemplateClass + L'<' + result + L'>';
   }

   return result;
}

String ASTType::toOpenGL() const
{
   switch ( mType )
   {
   case ASTType::eFloat2:
      return sVec2;
   case ASTType::eFloat3:
      return sVec3;
   case ASTType::eFloat4:
      return sVec4;
   case ASTType::eMat2:
      return sMat2;
   case ASTType::eMat3:
      return sMat3;
   case ASTType::eMat4:
      return sMat4;

   case ASTType::eVoid:
      return sVoid;
   case ASTType::eStruct:
      return mpStruct->mName;
   }

   return UTEXT("Unknown");
}