
#include "dxbuilder.h"

#include <vector>
#include <d3dcompiler.h>

#include "core/graphics/vertexlayout.h"
#include "core/graphics/vertexlayoutelement.h"
#include "core/conv/numberconverter.h"
#include "core/smartptr/autoptr.h"
#include "core/defines.h"

#include "../astannotation.h"
#include "../astbuffer.h"
#include "../asteffect.h"
#include "../aststruct.h"
#include "../aststructentry.h"
#include "../astfunction.h"
#include "../astfunctionargument.h"
#include "../astsampler.h"
#include "../asttechnique.h"
#include "../asttexture.h"
#include "../asttype.h"

void DxBuilder::buildVertexShader(const ASTEffect& effect, ASTTechnique& technique)
{
   const ASTFunction* pfunction = effect.findFunction(technique.mVertex.mEntry);
   if ( pfunction->mArguments.size() == 1 && pfunction->mArguments[0]->mpType->isStruct() )
   {
      const ASTStruct& input = pfunction->mArguments[0]->mpType->getStruct();
      technique.mpLayout = buildInputLayout(input);
   }

   String code = UTEXT("// generated vertex shader\n\n");
   
   String num;
   NumberConverter& conv = NumberConverter::getInstance();
   for ( std::size_t index = 0; index < effect.mBuffers.size(); ++index )
   {
      const ASTBuffer* pbuffer = effect.mBuffers[index];
      code += UTEXT("cbuffer ") + pbuffer->mName;

      int reg = pbuffer->mRegister;
      if ( reg == -1 )
         reg = (int) index;
      
      code += UTEXT(" : register(b") + conv.format(num, reg) + UTEXT(")\n{") + pbuffer->mBody + UTEXT("};\n\n");
   }

   code += buildStructs(effect, *pfunction);
   code += buildFunction(*pfunction);

   std::string data = code.toUtf8();
   std::string entry = technique.mVertex.mEntry.toUtf8();
   std::string target = technique.mVertex.mTarget.toUtf8();
   uint32_t flags = D3DCOMPILE_ENABLE_STRICTNESS;

   ID3DBlob *presult, *perror;
   HRESULT hr = D3DCompile(data.c_str(), data.length(), NULL, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry.c_str(), target.c_str(), flags, 0, &presult, &perror);
   if ( FAILED(hr) )
   {
      std::string d3derror = "Shader compile error: " + std::string((const char*)perror->GetBufferPointer());
      throw std::exception(d3derror.c_str());
   }

   technique.mVertex.mCompiledCode.writeBlob(presult->GetBufferPointer(), presult->GetBufferSize());
}

void DxBuilder::buildGeometryShader(const ASTEffect& effect, ASTTechnique& technique)
{
   const ASTFunction* pfunction = effect.findFunction(technique.mGeometry.mEntry);

   String code = UTEXT("// generated geometry shader\n\n");

   String num;
   NumberConverter& conv = NumberConverter::getInstance();
   for ( std::size_t index = 0; index < effect.mBuffers.size(); ++index )
   {
      const ASTBuffer* pbuffer = effect.mBuffers[index];
      code += UTEXT("cbuffer ") + pbuffer->mName;

      int reg = pbuffer->mRegister;
      if ( reg == -1 )
         reg = (int) index;
      
      code += UTEXT(" : register(b") + conv.format(num, reg) + UTEXT(")\n{") + pbuffer->mBody + UTEXT("};\n\n");
   }

   code += buildStructs(effect, *pfunction);
   code += buildFunction(*pfunction);

   std::string data = code.toUtf8();
   std::string entry = technique.mGeometry.mEntry.toUtf8();
   std::string target = technique.mGeometry.mTarget.toUtf8();
   uint32_t flags = D3DCOMPILE_ENABLE_STRICTNESS;

   ID3DBlob *presult, *perror;
   HRESULT hr = D3DCompile(data.c_str(), data.length(), NULL, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry.c_str(), target.c_str(), flags, 0, &presult, &perror);
   if ( FAILED(hr) )
   {
      std::string d3derror = "Shader compile error: " + std::string((const char*)perror->GetBufferPointer());
      throw std::exception(d3derror.c_str());
   }

   technique.mGeometry.mCompiledCode.writeBlob(presult->GetBufferPointer(), presult->GetBufferSize());
}

void DxBuilder::buildPixelShader(const ASTEffect& effect, ASTTechnique& technique)
{
   const ASTFunction* pfunction = effect.findFunction(technique.mPixel.mEntry);

   String code = UTEXT("// generated pixel shader\n\n");

   String num;
   NumberConverter& conv = NumberConverter::getInstance();
   for ( std::size_t index = 0; index < effect.mTextures.size(); ++index )
   {
      ASTTexture* ptexture = effect.mTextures[index];
      code += UTEXT("Texture2D ") + ptexture->mName + UTEXT(" : register(t");

      int reg = ptexture->mRegister;
      if ( reg == -1 )
      {
         reg = (int) index;
      }
      
      code += conv.format(num, reg) + UTEXT(");\n");
      num = UTEXT("");
   }

   for ( std::size_t index = 0; index < effect.mSamplers.size(); ++index )
   {
      ASTSampler* psampler = effect.mSamplers[index];
      code += UTEXT("SamplerState ") + psampler->mName + UTEXT(" : register(s");
      
      int reg = psampler->mRegister;
      if ( reg == -1 )
      {
         reg = index;
      }
      
      code += conv.format(num, (int)reg) + UTEXT(");\n");
      num = UTEXT("");
   }

   code += buildStructs(effect, *pfunction);
   code += buildFunction(*pfunction);

   std::string data = code.toUtf8();
   std::string entry = technique.mPixel.mEntry.toUtf8();
   std::string target = technique.mPixel.mTarget.toUtf8();
   uint32_t flags = D3DCOMPILE_ENABLE_STRICTNESS;

   ID3DBlob *presult, *perror;
   HRESULT hr = D3DCompile(data.c_str(), data.length(), NULL, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry.c_str(), target.c_str(), flags, 0, &presult, &perror);
   if ( FAILED(hr) )
   {
      std::string d3derror = "Shader compile error: " + std::string((const char*)perror->GetBufferPointer());
      throw std::exception(d3derror.c_str());
   }
      
   technique.mPixel.mCompiledCode.writeBlob(presult->GetBufferPointer(), presult->GetBufferSize());
}


String DxBuilder::buildStructs(const ASTEffect& effect, const ASTFunction& function)
{
   typedef std::vector<const ASTStruct*> Structs;
   Structs structs;

   if ( function.mpType->isStruct() )
   {
      const ASTStruct& aststruct = function.mpType->getStruct();
      structs.push_back(&aststruct);
   }   
   
   for ( std::size_t index = 0; index < function.mArguments.size(); ++index )
   {
      const ASTFunctionArgument* parg = function.mArguments[index];
      if ( parg->mpType->isStruct() )
      {
         const ASTStruct& aststruct = parg->mpType->getStruct();
         structs.push_back(&aststruct);
      }
   }

   String code;
   for ( std::size_t index = 0; index < structs.size(); ++index )
   {
      const ASTStruct* pstruct = structs[index];
      code += buildStruct(*pstruct);
   }
   return code;
}

String DxBuilder::buildStruct(const ASTStruct& str)
{
   String result = UTEXT("struct ") + str.mName + UTEXT("\n{\n");
   for ( std::size_t index = 0; index < str.mEntries.size(); ++index )
   {
      const ASTStructEntry* pentry = str.mEntries[index];
      result += UTEXT("  ") + buildStructEntry(*pentry) + L'\n';
   }
   result += UTEXT("};\n\n");

   return result;
}

String DxBuilder::buildStructEntry(const ASTStructEntry& entry)
{
   String result = entry.ptype->toDirectX() + L' ' + entry.name;
   if ( !entry.location.isEmpty() )
   {
      result += UTEXT(" : ") + entry.location;
      if ( entry.slot != L'' )
      {
         result += entry.slot;
      }
   }
   result += L';';
   return result;
}

String DxBuilder::buildFunction(const ASTFunction& function)
{
   String code;

   const ASTAnnotation* panno = function.findAnnotation(UTEXT("maxvertexcount"));
   if ( panno != NULL )
   {
      code += UTEXT("[maxvertexcount({0})]").arg(0, panno->intvalue);
   }

   code += function.mpType->toDirectX() + ' ' + function.mName + '(';
   for ( std::size_t index = 0; index < function.mArguments.size(); ++index )
   {
      const ASTFunctionArgument* parg = function.mArguments[index];

      if ( index > 0 )
      {
         code += UTEXT(", ");
      }
      else
      {
         const ASTAnnotation* panno = function.findAnnotation(UTEXT("inputtype"));
         if ( panno != NULL )
         {
            code += panno->strvalue + L' ';
         }
      }

      if ( !parg->mpType->getTemplateClass().isEmpty() )
      {
         // hack!! maybe we should support for pre-argument values like in, inout, out
         code += UTEXT("inout ");
      }
      
      code += parg->mpType->toDirectX() + ' ' + parg->mName;
      if ( parg->mArraySize > -1 )
      {
         String conv;
         code += L'[';
         code += NumberConverter::getInstance().format(conv, parg->mArraySize) + L']'; 
      }
   }

   code += ')';

   if ( !function.mReturn.isEmpty() )
   {
      code += UTEXT(" : ") + function.mReturn;
   }
   
   code += UTEXT("\n{") + function.mBody + UTEXT("}\n");
   return code;
}
