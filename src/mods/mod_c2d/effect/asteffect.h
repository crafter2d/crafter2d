
#ifndef AST_EFFECT_H
#define AST_EFFECT_H

#include "astnode.h"

#include <vector>

#include "core/string/string.h"

#include "astfunction.h"
#include "aststruct.h"

namespace Graphics
{
   class VertexLayout;
}

class ASTBuffer;
class ASTSampler;
class ASTTechnique;
class ASTTexture;

class ASTEffect
{
public:
   typedef std::vector<ASTFunction*> Functions;
   typedef std::vector<ASTStruct*> Structs;
   typedef std::vector<ASTBuffer*> Buffers;
   typedef std::vector<ASTTechnique*> Techniques;
   typedef std::vector<ASTTexture*> Textures;
   typedef std::vector<ASTSampler*> Samplers;

   enum Language { eDirectX, eOpenGL, eUnsupported };

   ASTEffect();

 // get/set
   Language getLanguage() const;
   void     setLanguage(Language language);

 // query
   bool hasSupportedLanguage() const;

 // operations
   void addBuffer(ASTBuffer* pbuffer) { mBuffers.push_back(pbuffer); }
   void addStruct(ASTStruct* pstruct) { mStructs.push_back(pstruct); }
   void addFunction(ASTFunction* pfunction) { mFunctions.push_back(pfunction); }
   void addTechnique(ASTTechnique* ptechnique) { mTechniques.push_back(ptechnique); }
   void addTexture(ASTTexture* ptexture) { mTextures.push_back(ptexture); }
   void addSampler(ASTSampler* psampler) { mSamplers.push_back(psampler); }

 // finding
   const ASTStruct* findStruct(const String& name) const;
   const ASTFunction* findFunction(const String& name) const;

 // data
   Language mLanguage;
   Buffers mBuffers;
   Structs mStructs;
   Functions mFunctions;
   Techniques mTechniques;
   Textures mTextures;
   Samplers mSamplers;

   Graphics::VertexLayout* mpLayout;
};

inline
const ASTStruct* ASTEffect::findStruct(const String& name) const
{
   for ( std::size_t index = 0; index < mStructs.size(); ++index )
   {
      ASTStruct* pstruct = mStructs[index];
      if ( pstruct->mName == name )
      {
         return pstruct;
      }
   }
   return NULL;
}

inline
const ASTFunction* ASTEffect::findFunction(const String& name) const
{
   for ( std::size_t index = 0; index < mFunctions.size(); ++index )
   {
      const ASTFunction* pfunction = mFunctions[index];
      if ( pfunction->mName == name )
      {
         return pfunction;
      }
   }
   return NULL;
}

#endif // AST_EFFECT_H
