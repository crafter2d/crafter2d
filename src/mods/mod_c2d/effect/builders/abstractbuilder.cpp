
#include "abstractbuilder.h"

#include <memory>

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
      buildGeometryShader(effect, *ptechnique);
      buildPixelShader(effect, *ptechnique);
   }

   return true;
}

void AbstractBuilder::buildInputLayout(const ASTStruct& input, VertexLayout& layout)
{
   int pos = 0;
   for ( auto pentry : input.mEntries )
   {
      const auto& semantic = pentry->location.isEmpty() ? pentry->name : pentry->location;

      layout.emplace_back(semantic, pos, toNativeType(*pentry->ptype));

      pos += getTypeSize(*pentry->ptype);
   }

   layout.setStride(pos);
}

uint32_t AbstractBuilder::getTypeSize(const ASTType& type)
{
   switch (type.getType())
   {
   case ASTType::eUint:
      return sizeof(uint32_t);

   case ASTType::eFloat:
   case ASTType::eFloat2:
   case ASTType::eFloat3:
   case ASTType::eFloat4:
      return sizeof(float) * (1 + type.getType() - ASTType::eFloat);
   }
   return 0;
}
