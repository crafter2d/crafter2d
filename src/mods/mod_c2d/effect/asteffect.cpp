
#include "asteffect.h"

ASTEffect::ASTEffect():
   mLanguage(eUnsupported),
   mBuffers(),
   mStructs(),
   mFunctions(),
   mTechniques(),
   mTextures(),
   mSamplers(),
   mpLayout(nullptr)
{
}

// get/set

ASTEffect::Language ASTEffect::getLanguage() const
{
   return mLanguage;
}

void ASTEffect::setLanguage(Language language)
{
   mLanguage = language;
}

// - Query

bool ASTEffect::hasSupportedLanguage() const
{
   return mLanguage != eUnsupported;
}
