
#ifndef EFFECT_WRITER_H
#define EFFECT_WRITER_H

#include "core/content/contentwriter.h"

namespace Graphics
{
   class VertexLayout;
}

class ASTEffect;
class ASTTechnique;
class DataStream;

class EffectWriter : public c2d::ContentWriter
{
protected:

   virtual Result write(DataStream& stream, const String& filename) override;

private:

 // streaming
   void write(DataStream& stream, const ASTEffect& effect);
   void write(DataStream& stream, const ASTTechnique& technique);
   void write(DataStream& stream, const Graphics::VertexLayout& layout);
};

#endif // EFFECT_WRITER_H
