
#include "effectreader.h" 

#include <cstdint>

#include "core/graphics/device.h"
#include "core/graphics/effect.h"
#include "core/graphics/effecttechnique.h"
#include "core/graphics/vertexlayout.h"
#include "core/graphics/vertexlayoutelement.h"
#include "core/streams/datastream.h"
#include "core/streams/arraystream.h"
#include "core/defines.h"

using namespace Graphics;

IContent* EffectReader::read(DataStream& stream)
{
   Effect* presult = new Effect();

   uint32_t techniques;
   stream.readUint32(techniques);
   for ( uint32_t index = 0; index < techniques; ++index )
   {
      EffectTechnique* ptechnique = readTechnique(stream);
      if ( ptechnique != nullptr )
      {
         presult->addTechnique(ptechnique);
      }
   }

   BlendStateDesc desc(BlendStateDesc::BS_SRC_ALPHA, BlendStateDesc::BS_SRC_INV_ALPHA, true);
   BlendState* pblendstate = getGraphicsDevice().createBlendState(desc);
   presult->setBlendState(pblendstate);

   return presult;
}

EffectTechnique* EffectReader::readTechnique(DataStream& stream)
{
   String name;
   stream >> name;

   auto layout = readVertexLayout(stream);

   int vertexshaderlen;
   stream.readInt(vertexshaderlen);
   const char* pvertexShader = stream.readBlob(vertexshaderlen);

   int geometryshaderlen;
   stream.readInt(geometryshaderlen);
   const char* pgeometryShader = stream.readBlob(geometryshaderlen);
   
   int pixelshaderlen;
   stream.readInt(pixelshaderlen);
   const char* ppixelShader = stream.readBlob(pixelshaderlen);

   ArrayStream vertexshader(pvertexShader, vertexshaderlen);
   ArrayStream geometryshader(pgeometryShader, geometryshaderlen);
   ArrayStream pixelshader(ppixelShader, pixelshaderlen);

   Device& device = getGraphicsDevice();
   CodePath* ppath = device.createCodePath();
   ASSERT_PTR(ppath);

   if ( ppath->create(layout.release(), vertexshader, geometryshader, pixelshader) )
   {
      auto result = std::make_unique<EffectTechnique>();
      result->setCodePath(ppath);
      return result.release();
   }
   
   return nullptr;
}

std::unique_ptr<Graphics::VertexLayout> EffectReader::readVertexLayout(DataStream& stream)
{
   int size = 0, stride = 0;
   stream >> size >> stride;
   auto layout = std::make_unique<VertexLayout>(size);
   layout->setStride(stride);

   for ( VertexLayoutElement& element : *layout )
   {
      stream >> element.semantic >> element.pos >> element.type;
   }
   return layout;
}
