
#include "effectwriter.h"

#include <memory>
#include <stdexcept>

#include "core/streams/datastream.h"
#include "core/graphics/effect.h"
#include "core/graphics/effecttechnique.h"
#include "core/graphics/vertexlayout.h"
#include "core/graphics/vertexlayoutelement.h"
#include "core/vfs/stdiofile.h"
#include "core/defines.h"

#include "asteffect.h"
#include "asttechnique.h"

#include "effectfileparser.h"
#include "effectvalidator.h"
#include "effectshaderbuilder.h"
#include "effectwriter.h"

using namespace Graphics;

c2d::ContentWriter::Result EffectWriter::write(DataStream& stream, const String& filename)
{
   StdioFile file;
   if ( !file.open(filename, File::ERead | File::EText) )
      throw std::runtime_error("Could not open the file!");

   int size = file.size();
   char* pdata = new char[size+1];
   int read = file.read(pdata, size);
   pdata[read] = 0;
   String content = String::fromUtf8(pdata);
   delete[] pdata;

   EffectFileParser parser;
   std::unique_ptr<ASTEffect> asteffect(parser.parse(content));
   if ( !asteffect )
   {
      throw std::runtime_error("Could not parse the file.");
   }

   EffectValidator validator;
   validator.validate(*asteffect);

   EffectShaderBuilder shaderbuilder;
   shaderbuilder.build(*asteffect);
         
   write(stream, *asteffect);

   return eOk;
}
   
void EffectWriter::write(DataStream& stream, const ASTEffect& effect)
{
   stream.writeUint32(effect.mTechniques.size());

   for (const ASTTechnique* ptechnique : effect.mTechniques)
   {
      write(stream, *ptechnique);
   }
}

void EffectWriter::write(DataStream& stream, const ASTTechnique& technique)
{
   stream << technique.mName;

   write(stream, technique.mLayout);

   stream.write(technique.mVertex.mCompiledCode);
   stream.write(technique.mGeometry.mCompiledCode);
   stream.write(technique.mPixel.mCompiledCode);
}

void EffectWriter::write(DataStream& stream, const VertexLayout& layout)
{
   stream << layout.getSize() << layout.getStride();
   for (const VertexLayoutElement& element : layout )
   {      
      stream << element.semantic << element.pos << element.type;
   }
}
