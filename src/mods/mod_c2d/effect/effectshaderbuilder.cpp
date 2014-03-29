
#include "effectshaderbuilder.h"

#include <vector>
#include <d3dcompiler.h>

#include "core/graphics/vertexlayout.h"
#include "core/graphics/vertexlayoutelement.h"
#include "core/conv/numberconverter.h"
#include "core/smartptr/autoptr.h"
#include "core/defines.h"

#include "builders/dxbuilder.h"
#include "builders/oglbuilder.h"

#include "asteffect.h"

using namespace Graphics;

void EffectShaderBuilder::build(ASTEffect& effect)
{
   if ( !effect.mTechniques.empty() )
   {
      AutoPtr<AbstractBuilder> pbuilder = NULL;
      switch ( effect.getLanguage() )
      {
      case ASTEffect::eDirectX:
         pbuilder = new DxBuilder();
         break;
      case ASTEffect::eOpenGL:
         pbuilder = new OglBuilder();
         break;

      default:
         throw new std::exception("No language specified!");
      }

      pbuilder->build(effect);
   }
}
