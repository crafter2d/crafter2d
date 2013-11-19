
#include "effectvalidator.h"

#include "asteffect.h"
#include "astfunction.h"
#include "astfunctionargument.h"
#include "aststruct.h"
#include "asttechnique.h"
#include "asttype.h"

void EffectValidator::validate(const ASTEffect& effect)
{
   validateFunctions(effect);
   validateTechniques(effect);
}

void EffectValidator::validateFunctions(const ASTEffect& effect)
{
   for ( std::size_t index = 0; index < effect.mFunctions.size(); ++index )
   {
      const ASTFunction* pfunction = effect.mFunctions[index];
      
      for ( std::size_t arg = 0; arg < pfunction->mArguments.size(); ++arg )
      {
         const ASTFunctionArgument* parg = pfunction->mArguments[arg];
         if ( parg->mpType->isUnknown() )
         {
            String error = UTEXT("Argument ") + parg->mName + UTEXT(" of function ") + pfunction->mName + UTEXT(" has unknown type.");
            throw std::exception(error.toUtf8().c_str());
         }
      }

      if ( pfunction->mpType->isUnknown() )
      {
         String error = UTEXT("Could not resolve return type of function ") + pfunction->mName;
         throw std::exception(error.toUtf8().c_str());
      }
   }
}

void EffectValidator::validateTechniques(const ASTEffect& effect)
{
   for ( std::size_t index = 0; index < effect.mTechniques.size(); ++index )
   {
      const ASTTechnique* ptechnique = effect.mTechniques[index];

      if ( !ptechnique->mVertex.mEntry.isEmpty() )
      {
         const ASTFunction* pfunction = effect.findFunction(ptechnique->mVertex.mEntry);
         if ( pfunction == NULL )
         {
            String error = UTEXT("Can not resolve vertex shader function ") + ptechnique->mVertex.mEntry + UTEXT(" in technique ") + ptechnique->mName;
            throw std::exception(error.toUtf8().c_str());
         }
      }

      if ( !ptechnique->mPixel.mEntry.isEmpty() )
      {
         const ASTFunction* pfunction = effect.findFunction(ptechnique->mPixel.mEntry);
         if ( pfunction == NULL )
         {
            String error = UTEXT("Can not resolve vertex shader function ") + ptechnique->mPixel.mEntry + UTEXT(" in technique ") + ptechnique->mName;
            throw std::exception(error.toUtf8().c_str());
         }
      }
   }
}
