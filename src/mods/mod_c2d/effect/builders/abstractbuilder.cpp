
#include "abstractbuilder.h"

#include "core/graphics/vertexlayout.h"
#include "core/graphics/vertexlayoutelement.h"

#include "../asteffect.h"
#include "../astfunction.h"
#include "../astfunctionargument.h"
#include "../aststructentry.h"
#include "../asttechnique.h"
#include "../asttype.h"

using namespace Graphics;

AbstractBuilder::AbstractBuilder()
{
}

AbstractBuilder::~AbstractBuilder()
{
}

// - Building

bool AbstractBuilder::build(ASTEffect& effect)
{
   for ( std::size_t index = 0; index < effect.mTechniques.size(); ++index )
   {
      ASTTechnique* ptechnique = effect.mTechniques[index];

      buildVertexShader(effect, *ptechnique);
      buildPixelShader(effect, *ptechnique);
   }

   return true;
}

VertexLayout* AbstractBuilder::buildInputLayout(const ASTStruct& input)
{
   VertexLayout* playout = new VertexLayout();
   
   int offset = 0;
   for ( int index = 0; index < input.mEntries.size(); ++index )
   {
      const ASTStructEntry& entry = *input.mEntries[index];
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
      if ( entry.location.isEmpty() )
      {
         pelement->semantic = entry.name;
      }
      else
      {
         pelement->semantic = entry.location;
      }
      pelement->pos = offset;
         
      offset += sizeof(float) * pelement->size;

      playout->add(pelement);
   }

   return playout;
}
