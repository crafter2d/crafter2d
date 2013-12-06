
#include "effectshaderbuilder.h"

#include <vector>
#include <d3dcompiler.h>

#include "core/graphics/vertexlayout.h"
#include "core/graphics/vertexlayoutelement.h"
#include "core/conv/numberconverter.h"
#include "core/defines.h"

#include "astbuffer.h"
#include "asteffect.h"
#include "aststruct.h"
#include "aststructentry.h"
#include "astfunction.h"
#include "astfunctionargument.h"
#include "astsampler.h"
#include "asttechnique.h"
#include "asttexture.h"
#include "asttype.h"
#include "effectinputlayout.h"

using namespace Graphics;

void EffectShaderBuilder::build(ASTEffect& effect)
{
   if ( effect.mTechniques.size() > 0 )
   {
      // assume that all techniques have the same input vertex layout
      // can change this such that all techniques have their own layout
      ASTTechnique* ptechnique = effect.mTechniques[0];
      const ASTFunction* pfunction = effect.findFunction(ptechnique->mVertex.mEntry);
      

      // build & optionally compile the shaders
      for ( std::size_t index = 0; index < effect.mTechniques.size(); ++index )
      {
         ASTTechnique* ptechnique = effect.mTechniques[index];
         buildVertexShader(effect, *ptechnique);
         buildPixelShader(effect, *ptechnique);
      }
   }
}

void EffectShaderBuilder::buildVertexShader(const ASTEffect& effect, ASTTechnique& technique)
{
   const ASTFunction* pfunction = effect.findFunction(technique.mVertex.mEntry);
   
   String code = UTEXT("// generated vertex shader\n\n");

   technique.mpLayout = buildInputLayout(*pfunction);

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

VertexLayout* EffectShaderBuilder::buildInputLayout(const ASTFunction& function)
{
   VertexLayout* playout = new VertexLayout();

   if (function.mArguments.size() == 1 && function.mArguments[0]->mpType->isStruct())
   {   
      int offset = 0;
      // use structure to determine the input layout
      const ASTStruct& aststruct = function.mArguments[0]->mpType->getStruct();
      for ( int index = 0; index < aststruct.mEntries.size(); ++index )
      {
         const ASTStructEntry& entry = *aststruct.mEntries[index];
         VertexLayoutElement* pelement = new VertexLayoutElement();

         switch ( entry.ptype->getType() )
         {
         case ASTType::eFloat2:
            pelement->size = 2;
            break;
         case ASTType::eFloat3:
            pelement->size = 3;
            break;
         case ASTType::eFloat4:
            pelement->size = 4;
            break;
         }

         pelement->index = index;
         pelement->semantic = entry.location;
         pelement->pos = offset;
         
         offset += sizeof(float) * pelement->size;

         playout->add(pelement);
      }
   }

   return playout;
}

void EffectShaderBuilder::buildPixelShader(const ASTEffect& effect, ASTTechnique& technique)
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

String EffectShaderBuilder::buildStructs(const ASTEffect& effect, const ASTFunction& function)
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

String EffectShaderBuilder::buildStruct(const ASTStruct& str)
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

String EffectShaderBuilder::buildStructEntry(const ASTStructEntry& entry)
{
   String result = entry.ptype->toString() + L' ' + entry.name;
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

String EffectShaderBuilder::buildFunction(const ASTFunction& function)
{
   String code = function.mpType->toString() + ' ' + function.mName + '(';
   for ( std::size_t index = 0; index < function.mArguments.size(); ++index )
   {
      if ( index > 0 )
      {
         code += UTEXT(", ");
      }

      const ASTFunctionArgument* parg = function.mArguments[index];
      code += parg->mpType->toString() + ' ' + parg->mName;
   }

   code += ')';

   if ( !function.mReturn.isEmpty() )
   {
      code += UTEXT(" : ") + function.mReturn;
   }
   
   code += UTEXT("\n{") + function.mBody + UTEXT("}\n");
   return code;
}
