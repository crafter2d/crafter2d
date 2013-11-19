
#include "effectbuilder.h"

#include "core/graphics/effect.h"
#include "core/graphics/effecttechnique.h"
#include "core/smartptr/autoptr.h"
#include "core/defines.h"

#include "asteffect.h"
#include "astfunction.h"
#include "astfunctionargument.h"
#include "asttechnique.h"
#include "asttype.h"

using namespace Graphics;

Graphics::Effect* EffectBuilder::build(const ASTEffect& effect)
{
   AutoPtr<Effect> result = new Effect();

   for ( std::size_t index = 0; index < effect.mTechniques.size(); ++index )
   {
      const ASTTechnique* pasttechnique = effect.mTechniques[index];

      EffectTechnique* ptechnique = new EffectTechnique();

      const ASTFunction* pfunction = effect.findFunction(pasttechnique->mVertex.mEntry);
      ASSERT_PTR(pfunction);

      if ( pfunction->mArguments.size() == 1 )
      {
         // assume that it is a structure
         const ASTFunctionArgument* parg = pfunction->mArguments[0];
         if ( parg->mpType->isStruct() )
         {
            const ASTStruct& aststruct = parg->mpType->getStruct();
         }
      }
   }

   return result.release();
}
