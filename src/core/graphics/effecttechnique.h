
#ifndef EFFECT_TECHNIQUE_H
#define EFFECT_TECHNIQUE_H

#include "core/core_base.h"

class String;

namespace Graphics
{
   class CodePath;
   class Device;
   class RenderContext;
   class UniformBuffer;
   class VertexBuffer;

   class CORE_API EffectTechnique
   {
   public:

      EffectTechnique();
      ~EffectTechnique();

    // get/set
      void setCodePath(CodePath* ppath);

    // buffers
      UniformBuffer* createUniformBuffer(const String& name) const;
      VertexBuffer*  createVertexBuffer(Device& device, int length, int usage);

    // operations
      void enable(RenderContext& context);
      void setConstantBuffer(RenderContext& context, int index, const UniformBuffer& buffer);

   private:

    // data
      CodePath* mpCodePath; // owns
   };
}

#endif // EFFECT_TECHNIQUE_H
