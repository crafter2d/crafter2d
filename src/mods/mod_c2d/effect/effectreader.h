
#ifndef EFFECT_READER_H
#define EFFECT_READER_H

#include <memory>

#include "core/content/contentreader.h"

namespace Graphics
{
   class EffectTechnique;
   class VertexLayout;
}

class EffectReader : public c2d::ContentReader
{
public:

   virtual IContent* read(DataStream& stream) override;

private:

 // reading
   Graphics::EffectTechnique* readTechnique(DataStream& stream);
   std::unique_ptr<Graphics::VertexLayout> readVertexLayout(DataStream& stream);
};

#endif // EFFECT_READER_H
