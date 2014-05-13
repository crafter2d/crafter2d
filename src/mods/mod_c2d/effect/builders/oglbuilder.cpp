
#include "oglbuilder.h"

#include "../astbuffer.h"
#include "../asteffect.h"
#include "../aststructentry.h"
#include "../asttechnique.h"
#include "../asttype.h"
#include "../asttexture.h"

void OglBuilder::buildVertexShader(const ASTEffect& effect, ASTTechnique& technique)
{
   const ASTFunction* pfunction = effect.findFunction(technique.mVertex.mEntry);

   String code = UTEXT("#version 150\n");

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

}

void OglBuilder::buildPixelShader(const ASTEffect& effect, ASTTechnique& technique)
{
   const ASTFunction* pfunction = effect.findFunction(technique.mPixel.mEntry);

   String code = UTEXT("#version 150\n");

   code += buildTextures(effect, *pfunction);
   code += buildPixelStructs(effect, *pfunction);
   code += buildFunction(*pfunction);

   std::string source = code.toUtf8();
   technique.mPixel.mCompiledCode.writeBlob(source.c_str(), source.length() + 1);
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

// Vertex structures have input attributes (may not be in a block) and an output struct
String OglBuilder::buildVertexStructs(const ASTEffect& effect, ASTTechnique& technique, const ASTFunction& function)
{
   String result;
   for ( std::size_t index = 0; index < effect.mStructs.size(); ++index )
   {
      const ASTStruct* pstruct = effect.mStructs[index];

      if ( function.mBody.indexOf(pstruct->mName) != -1 )
      {
         // this is the output struct
         result += buildInputOutputStruct(*pstruct, UTEXT("out"));
      }
      else
      {
         for ( std::size_t edx = 0; edx < pstruct->mEntries.size(); ++edx )
         {
            const ASTStructEntry* pentry = pstruct->mEntries[edx];

            if ( function.mBody.indexOf(pentry->name) != -1 )
            {
               // seems this entry is used in the function body, so must be the input
               technique.mpLayout = buildInputLayout(*pstruct);

               result += buildInputStruct(*pstruct);
               break;
            }
         }
      }
   }
   return result;
}

// Pixel shaders only have input structs, so only look for names of the structs
String OglBuilder::buildPixelStructs(const ASTEffect& effect, const ASTFunction& function)
{
   String result;
   for ( std::size_t index = 0; index < effect.mStructs.size(); ++index )
   {
      const ASTStruct* pstruct = effect.mStructs[index];

      if ( function.mBody.indexOf(pstruct->mName) != -1 )
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
String OglBuilder::buildInputOutputStruct(const ASTStruct& str, const String& direction)
{
   String result = direction + L' ' + str.mName + UTEXT("Struct {\n");
   for ( std::size_t index = 0; index < str.mEntries.size(); ++index )
   {
      const ASTStructEntry* pentry = str.mEntries[index];
      result += UTEXT("  ") + pentry->ptype->toOpenGL() + L' ' + pentry->name + UTEXT(";\n");
   }
   result += UTEXT("} ") + str.mName + UTEXT(";\n\n");

   return result;
}
