
#include "effectreader.h" 

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
   stream.readUint(techniques);
   for ( uint32_t index = 0; index < techniques; ++index )
   {
      EffectTechnique* ptechnique = readTechnique(stream);
      if ( ptechnique != NULL )
      {
         presult->addTechnique(ptechnique);
      }
   }

   BlendStateDesc desc(BlendStateDesc::BS_SRC_ALPHA, BlendStateDesc::BS_SRC_INV_ALPHA, true);
   BlendState* pblendstate = getDevice().createBlendState(desc);
   presult->setBlendState(pblendstate);

   return presult;
}

EffectTechnique* EffectReader::readTechnique(DataStream& stream)
{
   String name;
   stream >> name;

   VertexLayout* playout = readVertexLayout(stream);

   int vertexshaderlen;
   stream.readInt(vertexshaderlen);
   char* pvertexshader = new char[vertexshaderlen];
   stream.readBlob(pvertexshader, vertexshaderlen);

   int pixelshaderlen;
   stream.readInt(pixelshaderlen);
   char* ppixelshader = new char[pixelshaderlen];
   stream.readBlob(ppixelshader, pixelshaderlen);

   ArrayStream vertexshader(pvertexshader, vertexshaderlen);
   ArrayStream pixelshader(ppixelshader, pixelshaderlen);

   Device& device = getDevice();
   CodePath* ppath = device.createCodePath();
   ASSERT_PTR(ppath);

   EffectTechnique* presult = NULL;

   if ( ppath->create(playout, vertexshader, pixelshader) )
   {
      presult = new EffectTechnique();
      presult->setCodePath(ppath);
   }

   delete[] pvertexshader;
   delete[] ppixelshader;

   return presult;
}

Graphics::VertexLayout* EffectReader::readVertexLayout(DataStream& stream)
{
   int size = 0;
   stream >> size;
   VertexLayout* playout = new VertexLayout();
   for ( int index = 0; index < size; ++index )
   {
      VertexLayoutElement* pelement = new VertexLayoutElement;
      stream >> pelement->pos >> pelement->size >> pelement->semantic;

      playout->add(pelement);
   }
   return playout;
}
