
#include "dxbuilder.h"

#include <vector>
#include <d3dcompiler.h>
#include <d3d11.h>

#include "core/graphics/vertexlayout.h"
#include "core/conv/numberconverter.h"
#include "core/defines.h"

#include "../astannotation.h"
#include "../astbuffer.h"
#include "../astdefine.h"
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
      buildInputLayout(input, technique.mLayout);
   }

   String code = UTEXT("// generated vertex shader\n\n");

   code += buildDefines(effect);
   
   String num;
   NumberConverter& conv = NumberConverter::getInstance();
   for ( size_t index = 0; index < effect.mBuffers.size(); ++index )
   {
      const ASTBuffer* pbuffer = effect.mBuffers[index];
      code += UTEXT("cbuffer ") + pbuffer->mName;

      size_t reg = pbuffer->mRegister;
      if ( reg == -1 )
         reg =  index;
      
      code += UTEXT(" : register(b") + conv.format(num, reg) + UTEXT(")\n{") + pbuffer->mBody + UTEXT("};\n\n");
   }

   code += buildStructs(effect, *pfunction);
   code += buildFunction(*pfunction);

   std::string data = code.toUtf8();
   std::string entry = technique.mVertex.mEntry.toUtf8();
   std::string target = technique.mVertex.mTarget.toUtf8();
   uint32_t flags = D3DCOMPILE_ENABLE_STRICTNESS;

   ID3DBlob *presult, *perror;
   HRESULT hr = D3DCompile(data.c_str(), data.length(), nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry.c_str(), target.c_str(), flags, 0, &presult, &perror);
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
   if ( pfunction == nullptr )
   {
      return;
   }

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
   HRESULT hr = D3DCompile(data.c_str(), data.length(), nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry.c_str(), target.c_str(), flags, 0, &presult, &perror);
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
   HRESULT hr = D3DCompile(data.c_str(), data.length(), nullptr, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry.c_str(), target.c_str(), flags, 0, &presult, &perror);
   if ( FAILED(hr) )
   {
      std::string d3derror = "Shader compile error: " + std::string((const char*)perror->GetBufferPointer());
      throw std::exception(d3derror.c_str());
   }
      
   technique.mPixel.mCompiledCode.writeBlob(presult->GetBufferPointer(), presult->GetBufferSize());
}

uint32_t DxBuilder::toNativeType(const ASTType& type)
{
   switch ( type.getType() )
   {
   case ASTType::eUint:
      return DXGI_FORMAT_R32_UINT;
   case ASTType::eFloat:
      return DXGI_FORMAT_R32_FLOAT;
   case ASTType::eFloat2:
      return DXGI_FORMAT_R32G32_FLOAT;
   case ASTType::eFloat3:
      return DXGI_FORMAT_R32G32B32_FLOAT;
   case ASTType::eFloat4:
      return DXGI_FORMAT_R32G32B32A32_FLOAT;
   }
   return 0;
}

String DxBuilder::buildDefines(const ASTEffect& effect)
{
   NumberConverter& conv = NumberConverter::getInstance();
   String result;
   String num;

   for (auto pdefine : effect.mDefines)
   {
      result += UTEXT("#define ") + pdefine->mName + UTEXT(" ") + conv.format(num, pdefine->mValue) + L'\n';
   }

   return result;
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
   
   for (const ASTFunctionArgument* parg : function.mArguments )
   {
      if ( parg->mpType->isStruct() )
      {
         size_t s = 0;
         const ASTStruct& aststruct = parg->mpType->getStruct();
         for ( ; s < structs.size(); ++s )
         {
            if ( structs[s] == &aststruct )
            {
               break;
            }
         }

         if ( s == structs.size() )
         {
            structs.push_back(&aststruct);
         }
      }
   }

   String code;
   for ( const ASTStruct* pstruct : structs )
   {
      code += buildStruct(*pstruct);
   }
   return code;
}

String DxBuilder::buildStruct(const ASTStruct& str)
{
   String result = UTEXT("struct ") + str.mName + UTEXT("\n{\n");
   for ( const ASTStructEntry* pentry : str.mEntries )
   {
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
      if ( entry.slot != 0 )
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
   if ( panno != nullptr )
   {
      code += UTEXT("[maxvertexcount({0})]").arg(0, panno->intvalue);
   }

   code += function.mpType->toDirectX() + ' ' + function.mName + '(';
   for ( const ASTFunctionArgument* parg : function.mArguments )
   {
      if ( parg != *function.mArguments.begin() )
      {
         code += UTEXT(", ");
      }
      else
      {
         const ASTAnnotation* panno = function.findAnnotation(UTEXT("inputtype"));
         if ( panno != nullptr )
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
