
#include "effectwriter.h"

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

bool EffectWriter::write(DataStream& stream, const String& filename)
{
   StdioFile file;
   if ( file.open(filename, File::ERead | File::EText) )
   {
      int size = file.size();
      char* pdata = new char[size+1];
      int read = file.read(pdata, size);
      pdata[read] = 0;
      String content = String::fromUtf8(pdata);
      delete[] pdata;

      try
      {
         EffectFileParser parser;
         ASTEffect* pasteffect = parser.parse(content);

         EffectValidator validator;
         validator.validate(*pasteffect);

         EffectShaderBuilder shaderbuilder;
         shaderbuilder.build(*pasteffect);
         
         write(stream, *pasteffect);
      }
      catch (std::exception& e)
      {
         printf(e.what());
         return false;
      }

      return true;
   }

   return true;
}
   
void EffectWriter::write(DataStream& stream, const ASTEffect& effect)
{
   stream.writeUint(effect.mTechniques.size());

   for ( std::size_t index = 0; index < effect.mTechniques.size(); ++index )
   {
      const ASTTechnique* ptechnique = effect.mTechniques[index];
      write(stream, *ptechnique);
   }
}

void EffectWriter::write(DataStream& stream, const ASTTechnique& technique)
{
   stream << technique.mName;

   write(stream, *technique.mpLayout);

   stream.write(technique.mVertex.mCompiledCode);
   stream.write(technique.mPixel.mCompiledCode);
}

void EffectWriter::write(DataStream& stream, const VertexLayout& layout)
{
   stream << layout.getSize();
   for ( int index = 0; index < layout.getSize(); ++index )
   {
      const VertexLayoutElement& element = layout[index];
      
      stream << element.pos << element.size << element.semantic;
   }
}
