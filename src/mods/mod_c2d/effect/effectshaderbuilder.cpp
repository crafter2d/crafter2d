
#include "effectshaderbuilder.h"

#include <memory>
#include <vector>

#include "core/graphics/vertexlayout.h"
#include "core/graphics/vertexlayoutelement.h"
#include "core/conv/numberconverter.h"
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
      std::unique_ptr<AbstractBuilder> builder;
      switch ( effect.getLanguage() )
      {
#ifdef WIN32
      case ASTEffect::eDirectX:
         builder.reset(new DxBuilder());
         break;
#endif
      case ASTEffect::eOpenGL:
         builder.reset(new OglBuilder());
         break;

      default:
         throw new std::runtime_error("No language specified!");
      }

      builder->build(effect);
   }
}
