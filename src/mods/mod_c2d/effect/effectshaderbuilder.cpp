
#include "effectshaderbuilder.h"

#include <vector>

#include "core/graphics/vertexlayout.h"
#include "core/graphics/vertexlayoutelement.h"
#include "core/conv/numberconverter.h"
#include "core/smartptr/autoptr.h"
#include "core/defines.h"

#ifdef WIN32
#include <d3dcompiler.h>
#include "builders/dxbuilder.h"
#endif
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
#ifdef WIN32
      case ASTEffect::eDirectX:
         pbuilder = new DxBuilder();
         break;
#endif
      case ASTEffect::eOpenGL:
         pbuilder = new OglBuilder();
         break;

      default:
         throw new std::runtime_error("No language specified!");
      }

      pbuilder->build(effect);
   }
}
