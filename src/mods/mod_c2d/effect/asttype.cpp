
#include "asttype.h"

#include "core/string/string.h"
#include "core/defines.h"

#include "asteffect.h"

static const String sFloat2(UTEXT("float2"));
static const String sFloat3(UTEXT("float3"));
static const String sFloat4(UTEXT("float4"));
static const String sMat2x2(UTEXT("float2x2"));
static const String sMat3x3(UTEXT("float3x3"));
static const String sMat3x4(UTEXT("float3x4"));
static const String sMat4x4(UTEXT("float4x4"));

// static 
ASTType* ASTType::fromString(const ASTEffect& effect, const String& typestr)
{
   ASTType* presult = new ASTType();

   if ( typestr == sFloat2 )
   {
      presult->mType = ASTType::eFloat2;
   }
   else if ( typestr == sFloat3 )
   {
      presult->mType = ASTType::eFloat3;
   }
   else if ( typestr == sFloat4 )
   {
      presult->mType = ASTType::eFloat4;
   }
   else if ( typestr == sMat2x2 )
   {
      presult->mType = ASTType::eMat2x2;
   }
   else if ( typestr == sMat3x3 )
   {
      presult->mType = ASTType::eMat3x3;
   }
   else if ( typestr == sMat3x4 )
   {
      presult->mType = ASTType::eMat4x4;
   }
   else if ( typestr == sMat4x4 )
   {
      presult->mType = ASTType::eMat3x4;
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
   mType(eUnknown)
{
}

ASTType::ASTType(Type type):
   mType(type)
{
}

// - Get/set

ASTType::Type ASTType::getType() const
{
   return mType;
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

String ASTType::toString() const
{
   if ( mType == ASTType::eFloat2 )
   {
      return sFloat2;
   }
   else if ( mType == ASTType::eFloat3 )
   {
      return sFloat3;
   }
   else if ( mType == ASTType::eFloat4 )
   {
      return sFloat4;
   }
   else if ( mType == ASTType::eMat2x2 )
   {
      return sMat2x2;
   }
   else if ( mType == ASTType::eMat3x3 )
   {
      return sMat3x3;
   }
   else if ( mType == ASTType::eMat3x4 )
   {
      return sMat3x4;
   } 
   else if ( mType == ASTType::eMat4x4 )
   {
      return sMat4x4;
   }
   else if ( mType == ASTType::eStruct )
   {
      return mpStruct->mName;
   }

   return UTEXT("Unknown");
}
