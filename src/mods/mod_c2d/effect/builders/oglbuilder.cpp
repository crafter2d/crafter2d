
#include "oglbuilder.h"

#include "core/defines.h"

#include "../astbuffer.h"
#include "../astdefine.h"
#include "../asteffect.h"
#include "../astfunctionargument.h"
#include "../aststructentry.h"
#include "../asttechnique.h"
#include "../asttype.h"
#include "../asttexture.h"

void OglBuilder::buildVertexShader(const ASTEffect& effect, ASTTechnique& technique)
{
   const ASTFunction* pfunction = effect.findFunction(technique.mVertex.mEntry);

   String code = UTEXT("#version 150\n");
   for ( auto pdefine : effect.mDefines )
   {
      code += String(L"#define {0} {1}\n").arg(0, pdefine->mName).arg(1, pdefine->mValue);
   }

   for ( std::size_t index = 0; index < effect.mBuffers.size(); ++index )
   {
      const ASTBuffer* pbuffer = effect.mBuffers[index];
      code += UTEXT("layout (std140) uniform ") + pbuffer->mName + UTEXT(" {") + pbuffer->mBody + UTEXT("};\n\n");
   }

   code += buildVertexStructs(effect, technique, *pfunction);
   code += buildFunction(*pfunction);

   std::string source = code.toUtf8();
   technique.mVertex.mCompiledCode.writeBlob(source.c_str(), source.length() + 1);
}

void OglBuilder::buildGeometryShader(const ASTEffect& effect, ASTTechnique& technique)
{
   const ASTFunction* pfunction = effect.findFunction(technique.mGeometry.mEntry);
   if ( pfunction != nullptr )
   {
      String code = UTEXT("#version 430\n");

      const ASTAnnotation* panno = pfunction->findAnnotation(UTEXT("maxvertexcount"));
      if ( panno == nullptr )
      {
         // code += UTEXT("[maxvertexcount({0})]").arg(0, panno->intvalue);
         return;
      }

      code += UTEXT("layout(points) in;\n");
      code += UTEXT("layout(triangle_strip, max_vertices=4) out;\n");

      for ( std::size_t index = 0; index < effect.mBuffers.size(); ++index )
      {
         const ASTBuffer* pbuffer = effect.mBuffers[index];
         code += UTEXT("layout (std140) uniform ") + pbuffer->mName + UTEXT(" {") + pbuffer->mBody + UTEXT("};\n\n");
      }

      code += buildGeometryStructs(effect, *pfunction);
      code += buildFunction(*pfunction);

      std::string source = code.toUtf8();
      technique.mGeometry.mCompiledCode.writeBlob(source.c_str(), source.length() + 1);
   }
}

void OglBuilder::buildPixelShader(const ASTEffect& effect, ASTTechnique& technique)
{
   const ASTFunction* pfunction = effect.findFunction(technique.mPixel.mEntry);

   String code = UTEXT("#version 430\n");

   code += buildTextures(effect, *pfunction);
   code += buildPixelStructs(effect, *pfunction);
   code += buildFunction(*pfunction);

   std::string source = code.toUtf8();
   technique.mPixel.mCompiledCode.writeBlob(source.c_str(), source.length() + 1);
}

uint32_t OglBuilder::toNativeType(const ASTType& type)
{
   switch ( type.getType() )
   {
   case ASTType::eFloat:
      return 1;
   case ASTType::eFloat2:
      return 2;
   case ASTType::eFloat3:
      return 3;
   case ASTType::eFloat4:
      return 4;
   case ASTType::eUint:
      return 5;
   }
   return 0;
}

// OpenGL uses samplers to access the texture.
String OglBuilder::buildTextures(const ASTEffect& effect, const ASTFunction& function)
{
   String result;
   for ( std::size_t index = 0; index < effect.mTextures.size(); ++index )
   {
      ASTTexture* ptexture = effect.mTextures[index];
      result += UTEXT("uniform sampler2D ") + ptexture->mName + UTEXT(";\n");
   }
   result += L'\n';
   return result;
}

int findIndexOf(const String& src, const String& find)
{
   std::size_t index = 0;
   do
   {
      index = src.indexOf(find, index + 1);
      if ( index == String::npos )
         break;

      if ( !Char::isAlphaNum(src[index + find.length()]) )
         break;

   } while ( true );

   return index;
}

// Vertex structures have input attributes (may not be in a block) and an output struct
String OglBuilder::buildVertexStructs(const ASTEffect& effect, ASTTechnique& technique, const ASTFunction& function)
{
   String result;
   for ( auto pstruct : effect.mStructs )
   {
      if ( pstruct->mName == function.mArguments[0]->mpType->getStruct().mName )
      {
         // input struct
         buildInputLayout(*pstruct, technique.mLayout);

         result += buildInputStruct(*pstruct);
      }

      if ( pstruct->mName == function.mpType->getStruct().mName )
      {
         result += buildInputOutputStruct(*pstruct, UTEXT("out"));
      }
      /*
      // check if this struct is used as input attributes
      for ( auto pentry : pstruct->mEntries )
      {
         int index = findIndexOf(function.mBody, pentry->name);
         if ( index > 0 && function.mBody[index - 1] != L'.' )
         {
            // seems this entry is used in the function body, so must be the input
            buildInputLayout(*pstruct, technique.mLayout);

            result += buildInputStruct(*pstruct);

            break;
         }
      }

      if ( function.mBody.indexOf(pstruct->mName) != String::npos )
      {
         // this is the output struct
         result += buildInputOutputStruct(*pstruct, UTEXT("out"));
      }*/
   }
   return result;
}

String OglBuilder::buildGeometryStructs(const ASTEffect& effect, const ASTFunction& function)
{
   String result;

   if ( function.mArguments.size() != 2 )
   {
      // invalid, requires arg1 = input & arg2 = output
      return result;
   }

   ASTFunctionArgument* parg = function.mArguments[0];
   ASSERT(parg->mpType->isStruct());
   const ASTStruct& inputstruct = parg->mpType->getStruct();
   result += buildInputOutputStruct(inputstruct, UTEXT("in"), true);

   parg = function.mArguments[1];
   ASSERT(parg->mpType->isStruct());
   result += buildInputOutputStruct(parg->mpType->getStruct(), UTEXT("out"));

   return result;
}

// Pixel shaders only have input structs, so only look for names of the structs
String OglBuilder::buildPixelStructs(const ASTEffect& effect, const ASTFunction& function)
{
   String result;
   for ( std::size_t index = 0; index < effect.mStructs.size(); ++index )
   {
      const ASTStruct* pstruct = effect.mStructs[index];

      if ( function.mBody.indexOf(pstruct->mName) != String::npos )
      {
         result += buildInputOutputStruct(*pstruct, UTEXT("in"));
      }
   }
  
   result += UTEXT("out vec4 frag;\n\n");
   return result;
}

String OglBuilder::buildFunction(const ASTFunction& function)
{
   String result = UTEXT("void main()\n{") + function.mBody + UTEXT("}\n");
   return result;
}

// Exports the input of the vertex shader as separate in statements, as blocks
// are not supported (see OpenGL wiki).
String OglBuilder::buildInputStruct(const ASTStruct& str)
{
   String result;
   for ( std::size_t index = 0; index < str.mEntries.size(); ++index )
   {
      const ASTStructEntry* pentry = str.mEntries[index];
      result += UTEXT("in ") + pentry->ptype->toOpenGL() + L' ' + pentry->name + UTEXT(";\n");
   }
   result += L'\n';
   return result;
}

// Exports the struct to an output uniform.
String OglBuilder::buildInputOutputStruct(const ASTStruct& str, const String& direction, bool asarray)
{
   String result = direction + L' ' + str.mName + UTEXT("Struct {\n");
   for ( std::size_t index = 0; index < str.mEntries.size(); ++index )
   {
      const ASTStructEntry* pentry = str.mEntries[index];
      result += UTEXT("  ") + pentry->ptype->toOpenGL() + L' ' + pentry->name + UTEXT(";\n");
   }
   result += UTEXT("} ") + str.mName;
   if ( asarray )
   {
      result += UTEXT("[]");
   }
   
   result += UTEXT(";\n\n");

   return result;
}
